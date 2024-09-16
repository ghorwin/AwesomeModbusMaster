#ifndef MessageHandlerH
#define MessageHandlerH

#include <QObject>
#include <QMutex>

#include <IBK_MessageHandler.h>

class QFile;

/*! When instantiated, this class installs a new message handler for usage with qDebug() and similar.
	When receiving a message, it relais the message to log file (if configured), appends a formatted message to
	a log window (if configured), and emits a signal.

	Use qCDebug(logCategory) and similar functions. If you want logfile output, open log file with openLogFile()
	and set the requestedLogFileVerbosity > 0.

	This object also catches messages from the IBK library and handles them just like the Qt messages.
*/
class MessageHandler : public QObject, public IBK::MessageHandler {
	Q_OBJECT
public:
	/*! Returns the instance of the message handler singleton. */
	static MessageHandler & instance();

	explicit MessageHandler(QObject *parent = nullptr, bool enableUtf8OutputOnWindows=true);
	virtual ~MessageHandler() override;

	/*! Handler function for IBK::IBK_message() calls. */
	void msg(const std::string & msgText, IBK::msg_type_t t, const char * func_id, int verbose_level) override;

	/*! Create/open log file. Closes any previously opened logfiles.
		You can use this function to recreate a log file from scratch if it grows too large.
		\param logFilePath	Full path to logfile (directory must exist). Pass an empty string to close the current
							logfile and disable logfile usage.
		\param append		If true, the logfile will be opened in append-mode, otherwise
							it will be opened in truncation mode.
		\return Returns true if logfile was successfully opened for writing, otherwise false.
	*/
	bool openLogFile(const std::string & logFilename, bool append, std::string & errmsg) override;

	/*! This function is called when qDebug(), qWarning() etc is used in the code. */
	virtual void msg(const QtMsgType type, const QMessageLogContext &context, const QString &msg);

	// *** Configuration Properties ***

	/*! Holds level of detail that defines which messages should be printed on the console.
		0: off, 1: critical/fatal only, 2: +warning, 3: +info, 4: +debug
	*/
	unsigned int	m_verbosityLevelConsole = 4;	// by default, print all messages to console


	/*! Holds level of detail that defines which messages should be written to logfile.
		\sa m_verbosityLevelConsole
	*/
	unsigned int	m_verbosityLevelLogfile = 2;	// by default, write only messages up to standard level 2
													// to file (errors, warnings, but no info/debug messages)

	/*! If set to true, the logfile output includes the function signature. */
	bool			m_includeFuncSignature = false;

	/*! Number of indentation characters for context (file/function signature) string. */
	unsigned int	m_contextIndentation = 40;

	/*! Time stamp format to be prepended to the message (see QDate::toString() and QTime::toString()).
		\code
			m_timeStampFormat = "yyyy-MM-dd hh:mm:ss";
			// gives: "2000-12-25 07:20:15"
		\endcode
		Set to empty time stamp to disable time stamp output in logfile.
	*/
	QString			m_timeStampFormat = "yyyy-MM-dd hh:mm:ss";

	/*! If true, colored text will be written into the log window. */
	bool			m_coloredText = true;



	// *** STATIC FUNCTIONS ***

	/*! Removes quotes around strings (those are added automatically by Qt when dumping QString). */
	static QString stripQuotes(const QString & msg);

signals:
	/*! Emitted whenever a message was received and if m_sendSignal is true.
		Shall be connected to the log window to display the message.
	*/
	void msgReceived(const QtMsgType t, QString timeStamp, QString category, QString context, QString msg);


private:
	/*! Sets up console on Windows for UTF8 string output. */
	void setupUtf8Console();

	/*! Helper function for MessageHandler to print out utf8 strings onto the windows terminal. */
	void printUtf8(const std::string & utf8);

	/*! Helper function for MessageHandler to print out utf8 strings.
		Basically behaves as printUtf8, but writes to std::cerr instead of std::cout
	*/
	void printErrorUtf8(const std::string & utf8);


	/*! The global pointer to the UitMessageHandler object.
		This pointer is set in the constructor, and cleared in the destructor.
	*/
	static MessageHandler			*m_self;

	/*! Log file name. */
	QString							m_logfileName;

	/*! Stores original message handler. */
	QtMessageHandler				m_oldMsgHandler = nullptr;

	QMutex		m_mutex;
#ifdef _WIN32
	/*! Here we store the original console code page on Windows. */
	unsigned int					m_originalConsoleCP;
	bool							m_enableUtf8OutputOnWindows;
#endif

};


#endif // MessageHandlerH
