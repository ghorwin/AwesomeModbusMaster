#include "MessageHandler.h"

#include <iostream>

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QFileInfo>
#include <QDateTime>

#ifdef _WIN32
#include <Windows.h>
#include <cstdio>
#include <fcntl.h>
#endif

#include "StringUtils.h"


void msgHandlerWrapper(const QtMsgType type, const QMessageLogContext &context, const QString &msg) {
	MessageHandler::instance().msg(type, context, msg);
}


// All terminal codes used for ANSI color writing.
extern const char * const TERMINAL_CODES[16];

/*! Foreground and background colors. */
enum ConsoleColor {
	CF_BLACK 			= 0x00,
	CF_BLUE				= 0x01,
	CF_GREEN			= 0x02,
	CF_CYAN 			= 0x03,
	CF_RED				= 0x04,
	CF_MAGENTA			= 0x05,
	CF_YELLOW			= 0x06,
	CF_GREY				= 0x07,
	CF_DARK_GREY		= 0x08,
	CF_BRIGHT_BLUE		= 0x09,
	CF_BRIGHT_GREEN		= 0x0A,
	CF_BRIGHT_CYAN		= 0x0B,
	CF_BRIGHT_RED 		= 0x0C,
	CF_BRIGHT_MAGENTA	= 0x0D,
	CF_BRIGHT_YELLOW	= 0x0E,
	CF_WHITE			= 0x0F,
	CB_BLACK 			= 0x00,
	CB_BLUE				= 0x10,
	CB_GREEN			= 0x20,
	CB_RED 				= 0x30,
	CB_MAGENTA			= 0x40,
	CB_CYAN				= 0x50,
	CB_YELLOW			= 0x60,
	CB_GREY				= 0x70,
	CB_DARK_GREY		= 0x80,
	CB_BRIGHT_BLUE		= 0x90,
	CB_BRIGHT_GREEN		= 0xA0,
	CB_BRIGHT_RED 		= 0xB0,
	CB_BRIGHT_MAGENTA	= 0xC0,
	CB_BRIGHT_CYAN		= 0xD0,
	CB_BRIGHT_YELLOW	= 0xE0,
	CB_WHITE			= 0xF0
};

void setConsoleTextColor(ConsoleColor c) {

#ifdef _WIN32
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  // Get handle to standard output
	SetConsoleTextAttribute(hConsole,static_cast<WORD>(c));
#else
	// terminal codes > 16 are bold fonts and must be mapped accordingly
	int i = static_cast<int>(c);
	if (i>=16)
		i = i >> 1; // also set bold flag?
	std::cerr << TERMINAL_CODES[i];
	std::cout << TERMINAL_CODES[i];
#endif

}

extern const char * const TERMINAL_CODES[16] = {
	"\033[22;30m", // black			CF_BLACK
	"\033[22;34m", // blue 			CF_BLUE
	"\033[22;32m", // green 		CF_GREEN
	"\033[22;36m", // cyan 			CF_CYAN
	"\033[22;31m", // red			CF_RED
	"\033[22;35m", // magenta		CF_MAGENTA
	"\033[22;33m", // brown			CF_YELLOW
	"\033[22;37m", // gray			CF_GREY
	"\033[01;30m", // dark gray 	CF_DARK_GREY
	"\033[01;34m", // light blue	CF_BRIGHT_BLUE
	"\033[01;32m", // light green	CF_BRIGHT_GREEN
	"\033[01;36m", // light cyan	CF_BRIGHT_CYAN
	"\033[01;31m", // light red		CF_BRIGHT_RED
	"\033[01;35m", // light magenta	CF_BRIGHT_MAGENTA
	"\033[01;33m", // yellow		CF_BRIGHT_YELLOW
	"\033[01;37m"  // white			CF_WHITE
};


// *** MessageHandler ***

MessageHandler * MessageHandler::m_self = nullptr;

MessageHandler & MessageHandler::instance() {
	Q_ASSERT_X(m_self != nullptr, "[MessageHandler::instance]", "You must create an instance of "
		"MessageHandler before accessing MessageHandler::instance()!");
	return *m_self;
}


MessageHandler::MessageHandler(QObject *parent, bool enableUtf8OutputOnWindows) :
	QObject(parent)
{
	(void)enableUtf8OutputOnWindows; // silence compiler warning
	// singleton check
	Q_ASSERT_X(m_self == nullptr, "[MessageHandler::MessageHandler]", "You must not create multiple instances of "
		"class MessageHandler!");
	m_self = this;
	// register our message handler wrapper function
	m_oldMsgHandler = qInstallMessageHandler(msgHandlerWrapper);

#ifdef _WIN32
	m_enableUtf8OutputOnWindows = enableUtf8OutputOnWindows;
	if (enableUtf8OutputOnWindows)
		setupUtf8Console();
#endif // _WIN32

}


MessageHandler::~MessageHandler() {
	// restore original message handler
	qInstallMessageHandler(m_oldMsgHandler);
	m_oldMsgHandler = nullptr;
	// clear out singleton pointer
	m_self = nullptr;

#ifdef _WIN32
#if !defined(__MINGW32__)
	SetConsoleOutputCP(m_originalConsoleCP);
#endif
#endif
}


void MessageHandler::msg(const std::string & msgText, IBK::msg_type_t t, const char * func_id, int /*verbose_level*/) {
	// forward call to our own msg function
	QMessageLogContext context;
	context.function = func_id;
	QtMsgType msgType;
	switch (t) {
		case IBK::MSG_PROGRESS:
		case IBK::MSG_CONTINUED:
			msgType = QtInfoMsg;
		break;
		case IBK::MSG_WARNING:
			msgType = QtWarningMsg;
		break;
		case IBK::MSG_ERROR:
			msgType = QtCriticalMsg;
		break;
		case IBK::MSG_DEBUG:
			msgType = QtDebugMsg;
		break;
	}

	msg(msgType, context, QString::fromStdString(msgText));
}


void MessageHandler::msg(const QtMsgType type, const QMessageLogContext & context, const QString & msg) {

	// manually filter out bogus warning message from Qt about  QWindowsWindow::setGeometry

	QMutexLocker locker(&m_mutex);

	if (msg.indexOf("QWindowsWindow::setGeometry") != -1)
		return;

	// collect and extract infos from context
	QString contextStr;
	if (context.file != nullptr) {
		QFileInfo f(QString(context.file));
		QString fname = f.fileName();
		contextStr = QString("%1:%2").arg(fname).arg(context.line);
	}
	if (context.function != nullptr && m_includeFuncSignature) {
		QString func = QString(context.function).replace("__cdecl ", "");
		contextStr += ", " + func;
	}
	contextStr = "[" + contextStr + "]";
	QString strippedMsg = stripQuotes(msg).trimmed();

	// replace encoded special chars
	strippedMsg = strippedMsg.replace("\\n", "\n");
	strippedMsg = strippedMsg.replace("\\t", "  ");
//	strippedMsg = strippedMsg.replace("\\\"", """");

	QString category = context.category ? context.category : "default"; // actually, category is always set
	QString messageTypeStr;
	unsigned int messageVerbosity = 0;
	switch (type) {
		case QtDebugMsg:	messageTypeStr = "[Debug]";		messageVerbosity = 4; break;
		case QtInfoMsg:		messageTypeStr = "[Info]";		messageVerbosity = 3; break;
		case QtWarningMsg:	messageTypeStr = "[Warning]";	messageVerbosity = 2; break;
		case QtCriticalMsg:	messageTypeStr = "[Critical]";	messageVerbosity = 1; break;
		case QtFatalMsg:	messageTypeStr = "[Fatal]";		messageVerbosity = 1; break;
	}

	QString timeStamp;
	if (!m_timeStampFormat.isEmpty()) {
		QDateTime dt = QDateTime::currentDateTime();
		timeStamp = dt.toString(m_timeStampFormat);
	}

	QString consoleTimeStamp = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

	// *** screen log ***

	if (m_verbosityLevelConsole >= messageVerbosity) {

		// for screen log we dump out a table-like info in format
		// [Warning]     short message

		QString consoleMsg = QString("%1 %2 %3\n")
				.arg(consoleTimeStamp)
				.arg(messageTypeStr,-12)        // negative field width: left-aligned
				.arg(strippedMsg);
		// TODO : In case of long messages, break line and dump out in following lines with indentation

		if (m_coloredText) {
			// console coloring
			switch (type) {
				case QtDebugMsg:	setConsoleTextColor(CF_BRIGHT_GREEN); break;
				case QtWarningMsg:	setConsoleTextColor(CF_BRIGHT_YELLOW); break;
				case QtInfoMsg:		break;
				case QtCriticalMsg:	setConsoleTextColor(CF_BRIGHT_RED); break;
				case QtFatalMsg:	setConsoleTextColor(CF_BRIGHT_MAGENTA); break;
			}
		}
		// write UTF8 text to console
		if (type == QtCriticalMsg || type == QtFatalMsg)
			printErrorUtf8(consoleMsg.toStdString());
		else
			printUtf8(consoleMsg.toStdString());
		if (m_coloredText) {
			// restore console colors
			setConsoleTextColor(CF_GREY);
		}
	}


	// *** log file ***

	if (m_verbosityLevelLogfile >= messageVerbosity && !m_logfileName.isEmpty()) {
		QString logMsg;
		if (!m_timeStampFormat.isEmpty())
			logMsg +=timeStamp + " ";
		logMsg += QString("%1 %2 %3 %4\n")
				.arg(messageTypeStr,-12)        // negative field width: left-aligned
				.arg("[" + category + "]", -20)
				.arg(contextStr, -(int)m_contextIndentation)
				.arg(strippedMsg);
		QFile logfile(m_logfileName);
		logfile.open(QFile::Append);
		QTextStream strm(&logfile);
		strm << logMsg;
	}


	// *** notification signal ***
	emit msgReceived(type, timeStamp, QString(category), contextStr, strippedMsg);


	// in case of fatal messages, also finish the application in the hard way
	if (type == QtFatalMsg) {
		printErrorUtf8("Fatal error, aborting application.");
		exit(1);
	}
}


QString MessageHandler::stripQuotes(const QString & msg) {
	// if message string is enclosed in ", strip them at front and back
	if (!msg.isEmpty()) {
		if (*msg.begin() == '\"' && *msg.rbegin() == '\"')
			return msg.mid(1, msg.count()-2);
	}
	return msg;
}


bool MessageHandler::openLogFile(const std::string & logFilename, bool append, std::string &) {
	QString logFilePath = QString::fromStdString(logFilename);
	if (logFilePath.isEmpty())
		return true;

	if (m_verbosityLevelLogfile == 0)
		return true; // no log file writing, function ends successfully, though no logfile was opened

	// ensure that callers do not accidentally use relative file path
	Q_ASSERT(!QFileInfo(logFilePath).isRelative());

	// create parent path, if not existing
	QDir logDir = QFileInfo(logFilePath).absoluteDir();
	if (!logDir.exists())
		logDir.mkpath(logDir.absolutePath());

	// create new file
	m_logfileName = logFilePath;
	QFile logfile(logFilePath);

	if (append) {
		if (!logfile.open(QFile::WriteOnly | QFile::Append)) {
			return false;
		}

		QTextStream strm(&logfile);
		strm << "\n----------- continued -------------\n";
	}
	else {
		// open and clear existing logfile
		if (!logfile.open(QFile::WriteOnly | QFile::Truncate)) {
			return false;
		}
	}

	// Note: we close the logfile on purpose here, so that we can access/delete the logfile while the application is running
	return true;
}


void MessageHandler::setupUtf8Console() {

#if _WIN32
	// store current console properties and restore at application end
	m_originalConsoleCP = GetConsoleOutputCP();

	// set font capable of showing unicode characters
	// this is only necessary on Windows 7 or older..., so we can comment it out for now
//	CONSOLE_FONT_INFOEX cfi;
//	cfi.cbSize = sizeof cfi;
//	cfi.nFont = 0;
//	cfi.dwFontSize.X = 0;
//	cfi.dwFontSize.Y = 16;
//	cfi.FontFamily = FF_DONTCARE;
//	cfi.FontWeight = FW_NORMAL;
//	std::wcscpy(cfi.FaceName, L"Consolas");
//	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

	// set console code page to support UTF8
#if defined(__MINGW32__)
	_setmode(_fileno(stdout), _O_U16TEXT);
#else
	SetConsoleOutputCP(CP_UTF8);
#endif

#endif // _WIN32

}


void MessageHandler::printUtf8(const std::string & utf8) {
#ifdef _WIN32
	if (!m_enableUtf8OutputOnWindows) {
		std::cout << utf8;
		std::cout.flush();
		return;
	}
#if defined(__MINGW32__)
	std::wcout << UTF8ToWstring(utf8);
#else
	// VC can handle UTF8 output to console directly
	std::wprintf(L"%S", utf8.c_str());
#endif
#else
	// Linux handles utf8 natively
	std::cout << utf8;
#endif
	std::cout.flush();
}

void MessageHandler::printErrorUtf8(const std::string & utf8) {
#ifdef _WIN32
	if (!m_enableUtf8OutputOnWindows) {
		std::cerr << utf8;
		std::cerr.flush();
		return;
	}
#if defined(__MINGW32__)
	std::wcerr << UTF8ToWstring(utf8);
#else
	// VC can handle UTF8 output to console directly
	std::fwprintf(stderr, L"%S", utf8.c_str());
#endif
#else
	// Linux handles utf8 natively
	std::cerr << utf8;
#endif
	std::cerr.flush();
}



