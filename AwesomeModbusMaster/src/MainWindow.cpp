#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileInfo>
#include <QCloseEvent>
#include <QTimer>
#include <QHBoxLayout>
#include <QDockWidget>

#include <IBK_messages.h>

#include "Settings.h"
#include "Constants.h"
#include "LogWidget.h"
#include "MessageHandler.h"
#include "WelcomeScreen.h"
#include "MainContentWindow.h"

MainWindow * MainWindow::m_self = nullptr;


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	m_ui(new Ui::MainWindow),
	m_undoStack(new QUndoStack(this))
{
	// store pointer to this object for global access
	m_self = this;

	m_ui->setupUi(this);

	// rest of initialization is done in setup()
	QTimer::singleShot(0, this, &MainWindow::setup);
}


MainWindow::~MainWindow() {
	delete m_ui;
	delete m_undoStack;

	m_self = nullptr;
}


bool MainWindow::saveProject() {
	on_actionFileSave_triggered();
	return	!ProjectHandler::instance().isModified() &&
			!ProjectHandler::instance().projectFile().isEmpty();
}



// *** public static functions ***

MainWindow & MainWindow::instance() {
	Q_ASSERT_X(m_self != nullptr, "[MainWindow::instance]",
			   "You must not access MainWindow::instance() when there is no MainWindow "
			   "instance (anylonger).");
	return *m_self;
}


void MainWindow::addUndoCommand(QUndoCommand * command) {
	MainWindow::instance().m_undoStack->push(command);
	// mark project as modified
	MainWindow::instance().updateWindowTitle();

}


// *** protected functions ***


void MainWindow::changeEvent(QEvent *event) {
	FUNCID(MainWindow::changeEvent);
	if (event->type() == QEvent::ActivationChange && this->isActiveWindow()){
		if (ProjectHandler::instance().isValid() && !ProjectHandler::instance().projectFile().isEmpty()) {
			// check for externally modified project file and trigger "reload" action
			QDateTime lastModified = QFileInfo(ProjectHandler::instance().projectFile()).lastModified();
			QDateTime lastReadTime = ProjectHandler::instance().lastReadTime();
			if (lastModified > lastReadTime) {
				IBK::IBK_Message(IBK::FormatString("Last read time '%1', last modified '%2', asking for update.\n")
								 .arg(lastReadTime.toString().toStdString())
								 .arg(lastModified.toString().toStdString()), IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_DEVELOPER);
				// update last read time to avoid duplicate call
				ProjectHandler::instance().updateLastReadTime();

				int res = QMessageBox::question(this, tr("Reload configuration file"),
												tr("The configuration file has been modified by an external application. "
												   "When reloading this configuration file all unsaved changes will be lost. "
												   "Reload modified configuration?"), QMessageBox::Yes | QMessageBox::No);
				if (res == QMessageBox::Yes) {
					// reload project
					m_projectHandler.reloadProject(this);
					IBK::IBK_Message(IBK::FormatString("New last read time '%1'.\n")
									 .arg(lastReadTime.toString().toStdString()), IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_DEVELOPER);
				}
			}
		}
	}

	QMainWindow::changeEvent(event);
}


void MainWindow::closeEvent(QCloseEvent * event) {
	// move input focus away from any input fields (to allow editingFinished() events to fire)
	setFocus();

	// remember current project
	if ( m_projectHandler.isValid() ) {
		// store current project file because closeProject will clear this
		QString currentProjectFile = m_projectHandler.projectFile();
		// make sure we have saved and closed our project
		if (!m_projectHandler.closeProject(this)) {
			// user must have cancelled, so prevent closing of the application
			event->ignore();
			return;
		}
		Settings::instance().m_lastProjectFile = currentProjectFile;
	}
	else {
		Settings::instance().m_lastProjectFile.clear();
	}

//	disconnect(m_threadPool[0], SIGNAL(finished()), this, SLOT(onWorkerThreadFinished()));
//	m_threadPool[0]->stop();
//	IBK::IBK_Message("Waiting for worker thread to finish.\n", IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_STANDARD);
	IBK::MessageHandlerRegistry::instance().setDefaultMsgHandler();
//	m_threadPool[0]->wait(1000);

	// store view settings
	// store list of visible dock widgets
	QStringList dockWidgetVisibility;

	// save user config and recent file list
	Settings::instance().write(saveGeometry(), saveState());

	event->accept();
}


void MainWindow::setup() {

	// *** setup welcome widget ***

	QHBoxLayout * mainLayout = new QHBoxLayout;
	m_welcomeScreen = new WelcomeScreen(this);
	mainLayout->addWidget(m_welcomeScreen);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	m_ui->centralwidget->setLayout(mainLayout);
	// updates colors and entire content of welcome page
	m_welcomeScreen->onStyleChanged();
	//m_welcomeScreen->setStyleSheet("QWidget {background-image: url(:/gfx/background/simvicus-background-mail.png);}");

	connect(m_welcomeScreen, &WelcomeScreen::newProjectClicked, this, &MainWindow::on_actionFileNew_triggered);
	connect(m_welcomeScreen, &WelcomeScreen::openProjectClicked, this, &MainWindow::on_actionFileOpen_triggered);
	connect(m_welcomeScreen, &WelcomeScreen::openProject, this, &MainWindow::onOpenProjectByFilename);
	connect(m_welcomeScreen, &WelcomeScreen::updateRecentList, this, &MainWindow::onUpdateRecentProjects);

	// *** connect to ProjectHandler signals ***

	connect(&m_projectHandler, &ProjectHandler::updateActions, this, &MainWindow::onUpdateActions);
	connect(&m_projectHandler, &ProjectHandler::updateRecentProjects, this, &MainWindow::onUpdateRecentProjects);


	// *** Main content widget

	m_mainContentWindow = new MainContentWindow(this);
	mainLayout->addWidget(m_mainContentWindow);

	// *** Log dock widget ***

	m_logWidget = new LogWidget(this);
	MessageHandler * msgHandler = dynamic_cast<MessageHandler *>(IBK::MessageHandlerRegistry::instance().messageHandler());
	connect(msgHandler, &MessageHandler::msgReceived, m_logWidget, &LogWidget::onMsgReceived);

	QDockWidget::DockWidgetFeature titleBarOption = QDockWidget::NoDockWidgetFeatures;
	m_logDockWidget = new QDockWidget(this);
	m_logDockWidget->setObjectName("LogDockWidget");
	m_logDockWidget->setContentsMargins(0,0,0,0);
	m_logDockWidget->setWindowTitle(tr("Application Log"));
	m_logDockWidget->setFeatures(QDockWidget::AllDockWidgetFeatures | titleBarOption);
	m_logDockWidget->setAllowedAreas(Qt::BottomDockWidgetArea);
	QAction * toggleAction = m_logDockWidget->toggleViewAction();
	m_ui->menu_View->addAction(toggleAction);
	connect(toggleAction, &QAction::toggled, this, &MainWindow::onDockWidgetToggled);
	m_logDockWidget->setWidget(m_logWidget);
	addDockWidget(Qt::BottomDockWidgetArea,m_logDockWidget);

	// *** create menu for recent files ***

	m_recentProjectsMenu = new QMenu(this);
	m_ui->actionFileRecentProjects->setMenu(m_recentProjectsMenu);
	onUpdateRecentProjects();

	// *** update action status ***

	// update enable/disable state of actions and visibility of welcome or content page
	onUpdateActions();
}


void MainWindow::onActionOpenRecentFile() {
	QAction *action = qobject_cast<QAction *>(sender());
	if (action) {
		// before closing the project, check if the project file exists
		QString fname = action->data().toString();
		onOpenProjectByFilename(fname);
	}
}


void MainWindow::onUpdateActions() {
	// purpose of this function is to update the view layout based on the existance of a project or none

	// do we have a project?
	bool have_project = m_projectHandler.isValid();
	// enable/disable all actions that require a project

	// *** Project-dependent actions ***

	m_ui->actionFileSave->setEnabled(have_project);
	m_ui->actionFileSaveAs->setEnabled(have_project);
	m_ui->actionFileReload->setEnabled(have_project);

	// We dont have NO PROJECT
	if (!have_project) {

		m_welcomeScreen->setVisible(true);
		m_ui->menuBar->setVisible(false);
		m_mainContentWindow->setVisible(false);

		// Dock-Widget is only visible when project is there
		m_logDockWidget->toggleViewAction()->setEnabled(false);
		m_logDockWidget->toggleViewAction()->blockSignals(true);
		m_logDockWidget->setVisible(false);
		m_logDockWidget->toggleViewAction()->blockSignals(false);

		// no project, no undo actions -> clearing undostack also disables undo actions
		m_undoStack->clear();

		// turn off main view first when project is gone

		setWindowTitle(QString("Awesome Modbus Master %1").arg(LONG_VERSION));

		m_welcomeScreen->updateWelcomePage();
	}

	// when we have A PROJECT
	else {
		// hide welcome screen and show main content window
		m_welcomeScreen->setVisible(false);
		m_ui->menuBar->setVisible(true);
		m_mainContentWindow->setVisible(true);

		m_logDockWidget->toggleViewAction()->setEnabled(true);
		m_logDockWidget->toggleViewAction()->blockSignals(true);
		m_logDockWidget->setVisible(Settings::instance().m_logWidgetVisible);
		m_logDockWidget->toggleViewAction()->blockSignals(false);

		updateWindowTitle();

		// TODO :
	}
}


void MainWindow::onUpdateRecentProjects() {
	// create actions for recent files if number of max. recent projects in settings
	// differs from current number
	if (m_recentProjectActions.count() != (int)Settings::instance().m_maxRecentProjects) {
		qDeleteAll(m_recentProjectActions);
		m_recentProjectActions.clear();
		for (int i = 0; i < (int)Settings::instance().m_maxRecentProjects; ++i) {
			QAction * a = new QAction(this);
			m_recentProjectActions.push_back(a);
			connect(m_recentProjectActions[i], SIGNAL(triggered()), this, SLOT(onActionOpenRecentFile()));
			m_recentProjectsMenu->addAction(m_recentProjectActions[i]);
		}
	}

	// disable recent file actions
	if (Settings::instance().m_recentProjects.isEmpty()) {
		m_ui->actionFileRecentProjects->setEnabled(false);
		return;
	}
	else {
		m_ui->actionFileRecentProjects->setEnabled(true);
		for (int i = 0, count = Settings::instance().m_recentProjects.count(); i < count; ++i) {
			m_recentProjectActions[i]->setText(Settings::instance().m_recentProjects[i]);
			m_recentProjectActions[i]->setData(Settings::instance().m_recentProjects[i]);
			m_recentProjectActions[i]->setVisible(true);
		}

		for (int i = Settings::instance().m_recentProjects.count(); i < (int)Settings::instance().m_maxRecentProjects; ++i) {
			m_recentProjectActions[i]->setVisible(false);
		}
	}
}


void MainWindow::onOpenProjectByFilename(const QString & filename) {
	QFile f1(filename);
	if (!f1.exists()) {
		QMessageBox::critical(this, tr("File not found"), tr("The file '%1' cannot be found or does not exist.").arg(filename));
		return;
	}
	// move input focus away from any input fields (to allow editingFinished() events to fire)
	setFocus();
	// we first need to close the current project
	if (!m_projectHandler.closeProject(this)) return;
	// then create a new project and try to load the file
	m_projectHandler.loadProject(this, filename, false);
}


void MainWindow::onDockWidgetToggled(bool visible) {
	// get sender
	QAction * toggleAction = qobject_cast<QAction*>(sender());
	if (m_logDockWidget->toggleViewAction() == toggleAction) {
		Settings::instance().m_logWidgetVisible  = visible;
	}
}



// *** private slots ***



void MainWindow::on_actionFileNew_triggered() {
	// move input focus away from any input fields (to allow editingFinished() events to fire)
	setFocus();
	// close project if we have one
	if (!m_projectHandler.closeProject(this)) // emits updateActions() if project was closed
		return;

	// create new project
	m_projectHandler.newProject(); // emits updateActions()
}


void MainWindow::on_actionFileOpen_triggered() {

}


void MainWindow::on_actionFileSave_triggered() {

}


void MainWindow::on_actionFileSaveAs_triggered() {

}


void MainWindow::on_actionFileQuit_triggered() {
	close(); // save config request is handled in close event
}


// *** private functions ***

void MainWindow::updateWindowTitle() {
	// no project file given?
	QString shortFileName, longFileName;
	if (m_projectHandler.projectFile().isEmpty()) {
		shortFileName = tr("unnamed%1").arg(Settings::instance().m_projectFileSuffix);
		longFileName = shortFileName;
	}
	else {
		shortFileName = QFileInfo(m_projectHandler.projectFile()).fileName();
		longFileName = m_projectHandler.projectFile();
	}
	if (m_projectHandler.isModified())
		shortFileName += "*";
	setWindowTitle(QString("Awesome Modbus Master %1 - %2").arg(LONG_VERSION, shortFileName));
}
