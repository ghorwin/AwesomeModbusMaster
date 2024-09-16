#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class QUndoCommand;
class QUndoStack;
class LogWidget;
class WelcomeScreen;
class MainContentWindow;

#include "ProjectHandler.h"

/*! Application's main window. */
class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow() override;

	/*! Public access function to save project file.
		\return Returns true if project was saved and project handler
				has now an unmodified project with valid project filename.
	*/
	bool saveProject();

	/*! Returns a pointer to the MainWindow instance.
		Only access this function during the lifetime of the
		MainWindow instance.
	*/
	static MainWindow & instance();

	/*! Adds an undo command to the global undo stack.
		Ownership of the command object will be transferred to the stack.
	*/
	static void addUndoCommand(QUndoCommand * command);

protected:
	/*! Checks if project file has been changed by external application. */
	void changeEvent(QEvent *event) override;
	/*! Does the confirm-saving-before-close stuff. */
	void closeEvent(QCloseEvent * event) override;

private slots:
	/*! Does the entire UI initialization.
		Triggered with slight delay from the event loop. Make sure no other events kick in before setup has
		completed!
	*/
	void setup();

	/*! Triggered when a recent file menu entry was clicked. */
	void onActionOpenRecentFile();

	/*! Updates the state of all actions based on the current condition of the project.
		This slot is connected to the signal updateActions() from ProjectHandler.
	*/
	void onUpdateActions();
	/*! Updates the menu entries in the 'Recent Projects' submenu.
		This is a slot because we need to update the menu with the actions
		a bit delayed. When the user clicks on a recent project menu entry, the
		loadProject() function is indirectly called which in turn calls
		updateRecentProjects(). Since the menu actions in the recent projects
		menu are deleted, this would mean that the action currently process is
		being deleted - causing a crash. Therefore we don't call updateRecentProjects()
		directly, but via a QTimer::singleShot() and thus ensure that the
		action handler function is completed before the action is touched.
	*/
	void onUpdateRecentProjects();

	/*! Opens a project with filename.
		Called from onActionOpenRecentFile() and from welcome screen.
	*/
	void onOpenProjectByFilename(const QString & filename);

	/*! To remember dock widget visibility state. */
	void onDockWidgetToggled(bool);

	// all menu action slots below

	void on_actionFileNew_triggered();
	void on_actionFileOpen_triggered();
	void on_actionFileSave_triggered();
	void on_actionFileSaveAs_triggered();
	void on_actionFileQuit_triggered();

private:
	/*! Updates the window title. */
	void updateWindowTitle();

	/*! Global pointer to main window instance.
		Initialized in the constructor of MainWindow and
		reset to nullptr in the destructor. So be sure that the main window
		exists before accessing MainWindow::instance()
	*/
	static MainWindow			*m_self;

	Ui::MainWindow				*m_ui;

	/*! The global undo stack in the program. */
	QUndoStack					*m_undoStack											= nullptr;
	/*! Menu for the recent projects entries. */
	QMenu						*m_recentProjectsMenu									= nullptr;
	/*! List with action objects for each recent project in the main menu. */
	QList<QAction*>				m_recentProjectActions;
	/*! The project handler that manages the actual project. */
	ProjectHandler				m_projectHandler;

	/*! The welcome screen. */
	WelcomeScreen				*m_welcomeScreen										= nullptr;

	/*! The actual content of the application. */
	MainContentWindow			*m_mainContentWindow									= nullptr;

	QAction						*m_undoAction											= nullptr;
	QAction						*m_redoAction											= nullptr;

	/*! Dock widget with the log. */
	QDockWidget					*m_logDockWidget										= nullptr;
	LogWidget					*m_logWidget											= nullptr;

};

#endif // MAINWINDOW_H
