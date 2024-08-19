#include "MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QSplashScreen>
#include <QTimer>

#include "Directories.h"
#include "Exception.h"
#include "MessageHandler.h"

#include "Constants.h"
#include "Settings.h"


int main(int argc, char *argv[]) {

	// general appearance properties, must be set before application object is created
#if QT_VERSION >= 0x050a00 && QT_VERSION < 0x060000
	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
#endif

	Directories::appname 		= "AwesomeModbusMaster";
	Directories::devdir 		= "AwesomeModbusMaster";
	Directories::packagename	= "AwesomeModbusMaster"; // Note: not needed, debian package building is not required, yet

		// We have to do this before our QApplication is initialized
#if QT_VERSION >= 0x050E00
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

	// create wrapped-QApplication class (to catch rogue exceptions)
	DebugApplication a(argc, argv);

	// *** Locale setup for Unix/Linux ***
#if defined(Q_OS_UNIX)
	setlocale(LC_NUMERIC,"C");
#endif

//	qApp->setWindowIcon(QIcon(":/gfx/icons/Logo_64.png"));
	qApp->setApplicationName(PROGRAM_NAME);

	// open scope so that all our own objects are destructed before Qt pulldown
	int res = 0;
	{
		// *** Create log file directory and setup message handler ***
		QDir baseDir;
		baseDir.mkpath(Directories::userDataDir());
		// Install our own Message Handler
		// - log file will be opened when settings have been read
		// - log window will be set when configured in settings and created in MainWindow
		MessageHandler msgHandler;
		msgHandler.m_verbosityLevelLogfile = 4; // all in logfile
		if (!msgHandler.openLogFile(Directories::globalLogFile(), false)) {
			qCritical() << "Cannot open logfile:" << Directories::globalLogFile();
			return EXIT_FAILURE;
		}

		// *** Create and initialize setting object of the application ***
		Settings settings(ORG_NAME, PROGRAM_NAME);
		settings.setDefaults();
		settings.read();

		// *** Style Init ***

//		Style style; // constructor sets up most of the initialization

		// *** Install translator ***
		QtExt::LanguageHandler::instance().setup(SVSettings::instance().m_organization,
												 SVSettings::instance().m_appName,
												 "VICUS" );
		if (argParser.hasOption("lang")) {
			std::string dummy = argParser.option("lang");
			QString langid = QString::fromStdString(dummy);
			if (langid != QtExt::LanguageHandler::instance().langId()) {
				IBK::IBK_Message( IBK::FormatString("Installing translator for language: '%1'.\n")
									.arg(langid.toStdString()),
									IBK::MSG_PROGRESS, FUNC_ID, IBK::VL_STANDARD);
				QtExt::LanguageHandler::instance().installTranslator(langid);
			}
		}
		else {
			QtExt::LanguageHandler::instance().installTranslator(QtExt::LanguageHandler::langId());
		}

		// set default language in IBK MultiLanguageString
		IBK::MultiLanguageString::m_language = QtExt::LanguageHandler::langId().toStdString();


		// open a scope to control life-time of our main window
		try {

#ifndef QT_DEBUG
			// *** Show splash screen ****
			QSplashScreen splash(QPixmap(":/gfx/logo/SplashScreen.png"), Qt::WindowStaysOnTopHint);
			QTimer::singleShot(8000, &splash, &QSplashScreen::close);
			splash.show();
#endif
			qApp->processEvents();

			// MainWindow setup
			MainWindow w;		// also creates the Database-singleton, so from here on we can use Database::instance()

			w.show();
			res = a.exec();

		} catch (Uit::Exception & ex) {
			ex.writeMsgStackToError();
			qCritical() << "Exception caught from main window initialization.";
		}
		// MainWindow went out of scope and is released
		// Settings have been written from MainWindow::closeEvent()
		// MessageHandler

	} // own scope ends here

	return res;
}
