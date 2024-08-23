#ifndef LanguageHandlerH
#define LanguageHandlerH

#include <QString>
#include <QCoreApplication>

class QTranslator;

/*! Central class that handles language switching.

	Uses Qt::Directories to find translation files.

	\code
	// example usage

	// first configure language handler with application settings
	LanguageHandler::setup("UIT", "Zeiterfassung 2.0", "Zeiterfassung");

	// install german translation, loads language file 'Zeiterfassung_de.qm'
	LanguageHandler::instance()->installTranslator("de");
	\endcode
*/
class LanguageHandler {
	Q_DISABLE_COPY(LanguageHandler)
public:
	/*! Returns an instance of the language handler singleton. */
	static LanguageHandler & instance();

	/*! Destructor, removes language handler objects. */
	~LanguageHandler();

	/*! Initializes the language handler with application-specific constants. */
	static void setup(const QString & organization, const QString & program, const QString & languageFilePrefix);

	/*! Returns current language ID. */
	static QString langId();

	/*! Sets the language Id in the settings object. */
	static void setLangId(QString id);

	/*! Installs the translator identified by langId and stores the
		language ID in the program settings. */
	void installTranslator(QString langId);

private:
	static QString		m_organization;
	static QString		m_program;
	static QString		m_languageFilePrefix;

	/*! The translater for the strings of the program itself. */
	QTranslator * applicationTranslator;
	/*! The translater for strings of the standard dialogs and other Qt library messages. */
	QTranslator * systemTranslator;

	/*! Constructor is private (static singleton).*/
	LanguageHandler();

};

#endif // LanguageHandlerH
