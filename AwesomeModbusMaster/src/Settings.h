/*! This file is based on IBK's QtExt::Settings class (LPGL Licensed). */

#ifndef SettingsH
#define SettingsH

#include <QCoreApplication>
#include <QStringList>
#include <QMap>
#include <QMessageBox>

// Note: we include Style.h here as it is used pretty much everywhere and
//       only includes QString and QColor itself
#include "Style.h"


/*! This class handles user settings.
	It provides a few convenience functions, like the "Do not show again" dialogs and
	the cross-platform "open external text editor" functionality.
*/
class Settings {
	Q_DECLARE_TR_FUNCTIONS(Settings)
	Q_DISABLE_COPY(Settings)
public:
	/*! Returns the instance of the settings class (to be accessed everywhere in the application). */
	static Settings & instance();

	/*! Standard constructor.
		\param organization Some string defining the group/organization/company (major registry root name).
		\param appName Some string defining the application name (second part of registry root name).

		You may only instantiate one instance of the settings object in your application. An attempt to
		create a second instance will raise an exception in the constructor.
	*/
	Settings(const QString & organization, const QString & appName);

	/*! Destructor. */
	virtual ~Settings();

	/*! Sets default options (after first program start). */
	virtual void setDefaults();

	/*! Reads the user specific config data.
		The data is read in the usual method supported by the various platforms.
		The default implementation reads and populates all member variables.
	*/
	virtual void read();

	/*! Reads the main window configuration properties.
		\param geometry A bytearray with the main window geometry (see QMainWindow::saveGeometry())
		\param state A bytearray with the main window state (toolbars, dockwidgets etc.)
					(see QMainWindow::saveState())
	*/
	virtual void readMainWindowSettings(QByteArray &geometry, QByteArray &state);

	/*! Writes the user specific config data.
		The data is writted in the usual method supported by the various platforms.
		The default implementation writes all member variables.
		\param geometry A bytearray with the main window geometry (see QMainWindow::saveGeometry())
		\param state A bytearray with the main window state (toolbars, dockwidgets etc.)
					(see QMainWindow::saveState())
	*/
	virtual void write(QByteArray geometry, QByteArray state);

	/*! Provides application wide style object used by the classes in UitCore and UitWidgets.
		Default style only provides functionality for loading application wide style sheet and stores members
		needed by UitCore and UitWidget classes.
		Re-implemented in derived Settings classes to provide application-specific style sheets.
	*/
	virtual Style * style() { return m_style; }

	/*! Convenience function, checks for defined text editor executable, than spawns an external
		process and shows the file in the text editor in a cross-plattform suitable manner.
	*/
	bool openFileInTextEditor(QWidget * parent, const QString & filepath) const;

	/*! Shows a dialog with "Do not show again" check box. If the "Do not show" property is already set,
		this function does nothing.
		Use this function instead of creating a QMessageBox::information() dialog yourself.
		Pass a suitable unique dialog identification string as doNotShowAgainDialogID for this dialog
		when calling this function.
	*/
	void showDoNotShowAgainMessage(QWidget * parent, const QString & doNotShowAgainDialogID,
										  const QString & title, const QString & msg);

	/*! Shows a dialog with "Do not show again" check box. If the "Do not show" property is already set,
		this function does nothing.
		Use this function instead of creating a QMessageBox::question() dialog yourself.
		Pass a suitable unique dialog identification string as doNotShowAgainDialogID for this dialog
		when calling this function.
		Result is the pressed button. By default the Yes and No button will be shown.
		If the "Do not show" property is already set the last answer will be returned.
	*/
	QMessageBox::StandardButton showDoNotShowAgainQuestion(QWidget * parent, const QString & doNotShowAgainDialogID,
														   const QString & title, const QString & msg,
														   QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No));

	/*! Shows a dialog with detailed text. */
	static QMessageBox::StandardButton showDetailedTextMessageBox(QWidget * parent, QMessageBox::Icon icon,
														   const QString & title, const QString & msg,
														   const QString & detailedText,
														   QMessageBox::StandardButtons buttons = QMessageBox::StandardButtons(QMessageBox::Ok));

	// ****** PUBLIC MEMBERS ************

	/*! Organization ID name (used for QSettings).
		\note This value is NOT stored in the configuration and must not be altered by the user.
	*/
	QString						m_organization;

	/*! Application name (used for QSettings).
		\note This value is NOT stored in the configuration and must not be altered by the user.
	*/
	QString						m_appName;

	/*! Holds the file path to the project that
		should be opened during startup.

		By default, this value is set from m_lastProjectFile which is
		retrieved from the settings (i.e. registry) in read().
		However, if a project file was specified
		on the command line, the parseCmdLine() function will set
		this file instead.
		The separate property is needed in case that the command-line specified
		filename is invalid. In this case the m_lastProjectFile property is
		preserved and a start of the application without command line will
		correctly load the last successfully loaded project.

		\note This value is NOT stored in the configuration and must not be altered by the user.
	*/
	QString						m_initialProjectFile;


	// *** Members below are stored in settings file ***

	/*! Maximum number of files in the MRU (most-recently-used) files list. */
	unsigned int				m_maxRecentProjects;

	/*! Entries of the MRU list. */
	QStringList					m_recentProjects;

	/*! Maximum number of undo steps. */
	unsigned int				m_maxNumUNDOSteps;

	/*! The file last opened in the UI.
		This value should be set by the application whenever a project
		file was successfully loaded or saved.
	*/
	QString						m_lastProjectFile;

	/*! Executable file name of external text editor.
		The text editor executable is determined automatically in setDefaults(),
		but can be customized by the user.
	*/
	QString						m_textEditorExecutable;

	/*! Current language ID (en, de, etc.)
		Can be interpreted by the application to load the respective translation files.
		This should be done before any UI component is instantiated.
		\note Language-ID might be empty, in which case the application should determine
			  the language ID automatically based on the current System locale.
	*/
	QString						m_langId;

	/*! If this is true, the native dialogs (Windows/Gnome/MacOS) for file and color dialogs
		will not be used, but the Qt-based dialog variants, instead.
		This might prevent platform specific compatibility issues (that exist with some Qt versions).
	*/
	bool						m_dontUseNativeDialogs;

	/*! Stores the logging threshold for console. */
	int							m_userLogLevelConsole;

	/*! Stores the logging threshold for the log file. */
	int							m_userLogLevelLogfile;

	/*! The version number stored during last run (used to detect version changes). */
	QString						m_lastVersionNumber;

	/*! Map of do-not-show-again dialog IDs and their checked states. */
	QMap<QString, bool>			m_doNotShowAgainDialogs;

	/*! Map of do-not-show-again dialog IDs and their last answer.
		Only used for messages with result.
	*/
	QMap<QString, QMessageBox::StandardButton>	m_doNotShowAgainDialogAnswers;

private:
	/*! The global pointer to the Settings object.
		This pointer is set in the constructor, and cleared in the destructor.
	*/
	static Settings			*m_self;

	/*! Application-central styling info.
		\note Aggregation by pointer is currently not necessary, as we include Style.h for convenience
			  above. However, if the style class implementation becomes "heavier" in future, we may drop
			  the include above and have proper implementation hiding, as it is good practice.
			  With the current design, we could aggregate by member directly, thus avoiding the allocation on
			  the heap, but with the pointer aggregation we are prepared for the future.
	*/
	Style					*m_style = nullptr;

};


#endif // SettingsH
