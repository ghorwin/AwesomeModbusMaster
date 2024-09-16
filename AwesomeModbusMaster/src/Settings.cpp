#include "Settings.h"

#include <QSettings>
#include <QProcess> // for open text editor
#include <QCheckBox>
#include <QFileInfo>
#include <QDebug>
#include <QLoggingCategory>

#include "MessageHandler.h"
#include "Directories.h"
#include "Constants.h"
#include "Style.h"

Settings * Settings::m_self = nullptr;

Settings & Settings::instance() {
	Q_ASSERT_X(m_self != nullptr, "[Settings::instance]", "You must create an instance of "
		"the Settings class before accessing Settings::instance()!");
	return *m_self;
}


Settings::Settings(const QString & organization, const QString & appName) :
	m_organization(organization),
	m_appName(appName),
	m_maxRecentProjects(10),
	m_maxNumUNDOSteps(50)
{
	// singleton check
	Q_ASSERT_X(m_self == nullptr, "[Settings::Settings]", "You must not create multiple instances of "
		"class Settings!");
	m_self = this;
	m_style = new Style();
}


Settings::~Settings() {
	// clear out singleton pointer
	m_self = nullptr;
	delete m_style;
}


void Settings::setDefaults() {
	// compose default values

	// clear last project file (if any, it is read in the read() function)
	m_lastProjectFile.clear();

	// set default log level, see MessageHandler
	m_userLogLevelConsole = 1;
	m_userLogLevelLogfile = 1;

	m_maxRecentProjects = 10;

	m_maxNumUNDOSteps = 10000;

	// determine text executable
	m_textEditorExecutable.clear();
#ifdef Q_OS_UNIX
	m_textEditorExecutable = "geany";
#elif defined(Q_OS_WIN)
	QStringList textEditorInstallLocations;
	textEditorInstallLocations
			<< "C:\\ProgramData\\chocolatey\\bin\\notepad++.exe"
			<< "C:\\Users\\All Users\\chocolatey\\bin\\notepad++.exe"
			<< "C:\\Program Files\\Notepad++\\notepad++.exe"
			<< "C:\\Program Files (x86)\\Notepad++\\notepad++.exe";

	m_textEditorExecutable.clear();
	for (const QString & installLoc : qAsConst(textEditorInstallLocations)) {
		if (QFileInfo::exists(installLoc)) {
			m_textEditorExecutable = installLoc;
			break;
		}
	}
#else
	// OS x editor?
#endif

	m_dontUseNativeDialogs = true;
	m_style->setStyle(Style::TT_Normal);
}


void Settings::read() {

	QSettings settings( m_organization, m_appName );

	m_lastProjectFile = settings.value("LastProjectFile", QString()).toString();
	m_recentProjects = settings.value("RecentProjects", QStringList()).toStringList();

	m_maxRecentProjects = settings.value("MaxRecentProjects", m_maxRecentProjects).toUInt();
	m_maxNumUNDOSteps = settings.value("MaxNumUndoSteps", m_maxNumUNDOSteps).toUInt();

	QString tmpTextEditorExecutable = settings.value("TextEditorExecutable", m_textEditorExecutable ).toString();
	if (!tmpTextEditorExecutable.isEmpty())
		m_textEditorExecutable = tmpTextEditorExecutable;
	m_langId = settings.value("LangID", m_langId ).toString(); // keep the default language ID, if no language ID was saved already

	m_userLogLevelConsole = settings.value("UserLogLevelConsole", m_userLogLevelConsole ).toInt();
	m_userLogLevelLogfile = settings.value("UserLogLevelLogfile", m_userLogLevelLogfile ).toInt();

	m_dontUseNativeDialogs = settings.value("DontUseNativeDialogs", m_dontUseNativeDialogs ).toBool();
	m_logWidgetVisible = settings.value("LogWidgetVisible", m_logWidgetVisible).toBool();

	int count = settings.beginReadArray("DoNotShowAgainDialogs");
	for (int i=0; i<count; ++i) {
		settings.setArrayIndex(i);
		QString s = settings.value("DialogID").toString();
		bool checked = settings.value("Checked", false).toBool();
		if (!s.isEmpty())
			m_doNotShowAgainDialogs[s] = checked;
	}
	settings.endArray();

	count = settings.beginReadArray("DoNotShowAgainDialogsAnswer");
	for (int i=0; i<count; ++i) {
		settings.setArrayIndex(i);
		QString s = settings.value("DialogID").toString();
		QMessageBox::StandardButton answer = static_cast<QMessageBox::StandardButton>(settings.value("Answer", QMessageBox::NoButton).toInt());
		if (!s.isEmpty())
			m_doNotShowAgainDialogAnswers[s] = answer;
	}
	settings.endArray();

	// *** Message Filter Rules

	// no debug messages for core routines like language handler or message handler
	QLoggingCategory::setFilterRules("core.*.debug=false\n"
									 "smtpclient.debug=false");

	// *** Message Handler log file
	std::string errmsg;
	if (!MessageHandler::instance().openLogFile(Directories::globalLogFile().toStdString(), false, errmsg)) {
		qCritical() << "Cannot open logfile:" << Directories::globalLogFile();
	}
	else {
		qDebug() << "Started writing log file:" << Directories::globalLogFile();
	}
}


void Settings::readMainWindowSettings(QByteArray &geometry, QByteArray &state) {

	QSettings settings( m_organization, m_appName );
	geometry = settings.value("MainWindowGeometry", QByteArray()).toByteArray();
	state = settings.value("MainWindowState", QByteArray()).toByteArray();

}


void Settings::write(QByteArray geometry, QByteArray state) {

	QSettings settings( m_organization, m_appName );
	settings.setValue("LastProjectFile", m_lastProjectFile);
	settings.setValue("RecentProjects", m_recentProjects);

	settings.setValue("MaxRecentProjects", m_maxRecentProjects );
	settings.setValue("UndoSize",m_maxNumUNDOSteps);

	settings.setValue("TextEditorExecutable", m_textEditorExecutable );
	settings.setValue("LangID", m_langId );

	settings.setValue("UserLogLevelConsole", m_userLogLevelConsole);
	settings.setValue("UserLogLevelLogfile", m_userLogLevelLogfile);

	settings.setValue("DontUseNativeDialogs", m_dontUseNativeDialogs );
	settings.setValue("LogWidgetVisible", m_logWidgetVisible );

	settings.setValue("MainWindowGeometry", geometry);
	settings.setValue("MainWindowState", state);

	settings.beginWriteArray("DoNotShowAgainDialogs");
	int i=0;
	for (QMap<QString, bool>::const_iterator it = m_doNotShowAgainDialogs.constBegin();
		 it != m_doNotShowAgainDialogs.constEnd(); ++it, ++i)
	{
		settings.setArrayIndex(i);
		settings.setValue("DialogID", it.key());
		settings.setValue("Checked", it.value());
	}
	settings.endArray();

	settings.beginWriteArray("DoNotShowAgainDialogsAnswer");
	i=0;
	for (QMap<QString, QMessageBox::StandardButton>::const_iterator it = m_doNotShowAgainDialogAnswers.constBegin();
		 it != m_doNotShowAgainDialogAnswers.constEnd(); ++it, ++i)
	{
		settings.setArrayIndex(i);
		settings.setValue("DialogID", it.key());
		settings.setValue("Answer", (int)it.value());
	}
	settings.endArray();
}


bool Settings::openFileInTextEditor(QWidget * parent, const QString & filepath) const {
	// check if editor has been set in preferences
	if (m_textEditorExecutable.isEmpty()) {
		QMessageBox::critical(parent, tr("Missing user preferences"), tr("Please open the preferences dialog and specify "
																	   "a text editor first!"));
		return false;
	}

	bool res = QProcess::startDetached( m_textEditorExecutable, QStringList() << filepath );
	if (!res) {
		QMessageBox::critical(parent, tr("Error starting external application"), tr("Text editor '%1' could not be started.")
							  .arg(m_textEditorExecutable));
	}
	return res;
}


void Settings::showDoNotShowAgainMessage(QWidget * parent, const QString & doNotShowAgainDialogID,
									  const QString & title, const QString & msg)
{
	if (!m_doNotShowAgainDialogs.contains(doNotShowAgainDialogID) || !m_doNotShowAgainDialogs[doNotShowAgainDialogID]) {
		QMessageBox msgBox(QMessageBox::Information, title, msg, QMessageBox::Ok, parent);
		msgBox.setCheckBox( new QCheckBox(tr("Do not show this dialog again.")) );
		msgBox.exec();
		if (msgBox.checkBox()->isChecked())
			m_doNotShowAgainDialogs[doNotShowAgainDialogID] = true;
	}
}


QMessageBox::StandardButton Settings::showDoNotShowAgainQuestion(QWidget * parent, const QString & doNotShowAgainDialogID,
																 const QString & title, const QString & msg,
																 QMessageBox::StandardButtons buttons)
{
	if (!m_doNotShowAgainDialogs.contains(doNotShowAgainDialogID) || !m_doNotShowAgainDialogs[doNotShowAgainDialogID]) {
		QMessageBox msgBox(QMessageBox::Question, title, msg, buttons, parent);
		msgBox.setCheckBox( new QCheckBox(tr("Do not show this dialog again.")) );
		QMessageBox::StandardButton res = static_cast<QMessageBox::StandardButton>(msgBox.exec());
		if (msgBox.checkBox()->isChecked()) {
			m_doNotShowAgainDialogs[doNotShowAgainDialogID] = true;
			m_doNotShowAgainDialogAnswers[doNotShowAgainDialogID] = res;
		}
		return res;
	}
	else {
		return m_doNotShowAgainDialogAnswers[doNotShowAgainDialogID];
	}
}


QMessageBox::StandardButton Settings::showDetailedTextMessageBox(QWidget * parent, QMessageBox::Icon icon,
		const QString & title, const QString & msg, const QString & detailedText, QMessageBox::StandardButtons buttons)
{
	QMessageBox msgBox(icon, title, msg, buttons, parent);
	msgBox.setDetailedText(detailedText);
	// search button with "ActionRole" button

	foreach (QAbstractButton *button, msgBox.buttons()) {
		if (msgBox.buttonRole(button) == QMessageBox::ActionRole) {
			button->click(); // found it, click to show detailed text
			break;
		}
	}
	return (QMessageBox::StandardButton)msgBox.exec();
}
