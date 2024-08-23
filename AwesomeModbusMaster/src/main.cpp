#include "MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QSplashScreen>
#include <QTimer>
#include <QDir>

#include "Directories.h"
#include "Exception.h"
#include "MessageHandler.h"
#include "LanguageHandler.h"
#include "Settings.h"

#include "Constants.h"
#include "DebugApplication.h"

int main(int argc, char *argv[]) {

	// configure directories class
	Directories::appname 		= "AwesomeModbusMaster";
	Directories::devdir 		= "AwesomeModbusMaster";
	Directories::packagename	= "AwesomeModbusMaster"; // Note: not needed, debian package building is not required, yet

	// general appearance properties, must be set before application object is created
#if QT_VERSION >= 0x050a00 && QT_VERSION < 0x060000
	QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
#endif

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


		// *** Install translator ***
		LanguageHandler::instance().setup(Settings::instance().m_organization,
											   Settings::instance().m_appName,
											   "AwesomeModbusMaster" );
		LanguageHandler::instance().installTranslator(LanguageHandler::langId());

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
			MainWindow w;

			w.show();
			res = a.exec();

		} catch (Exception & ex) {
			ex.writeMsgStackToError();
			qCritical() << "Exception caught from main window initialization.";
		}
		// MainWindow went out of scope and is released
		// Settings have been written from MainWindow::closeEvent()

	} // own scope ends here

	return res;
}
