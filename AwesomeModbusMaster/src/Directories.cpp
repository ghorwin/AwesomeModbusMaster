#include "Directories.h"

#include <QCoreApplication>
#include <QStandardPaths>

#include <QDir>
#include <QDebug>

#include <IBK_Exception.h> // for FUNC_ID macro

// You need to set values to these variables early on in your main.cpp
QString Directories::appname;			// for example "MasterSim"
QString Directories::devdir;			// for example "MasterSim" // git-repo-directory
QString Directories::packagename;		// for example "mastersim"


QString Directories::resourcesRootDir() {
	QString installPath = qApp->applicationDirPath();

#if defined(DEPLOYMENT) || defined(BUILD_DEBIAN_PACKAGE)
	// deployment mode

#if defined(Q_OS_WIN)
	// in Deployment mode, resources are inside the install directory
	return installPath;
#elif defined(Q_OS_MAC)
	// in deployment mode, we have them in <appname>.app/Contents/Resources
	// where install path is <appname>.app/MacOS
	return installPath + "/../Resources";
#elif defined(Q_OS_UNIX)

#ifdef BUILD_DEBIAN_PACKAGE

	// we install to /usr/bin/<appname>
	// and the package data is in
	//               /usr/share/<packagename>
	return installPath + "/../share/" + packagename;

#else // BUILD_DEBIAN_PACKAGE

	return installPath + "/../resources";

#endif // BUILD_DEBIAN_PACKAGE

#endif // defined(Q_OS_UNIX)


#else // DEPLOYMENT

	// development (IDE) mode

	// for shadow-builds the working directory is expected to be set to '<repo-root>/bin/debug'
#if defined(Q_OS_MAC)
	// in development mode, we have the resources outside the bundle
	return QFileInfo(installPath + "/../../../../" + devdir + "/resources").absoluteFilePath();
#else
	QFileInfo resPathInfo(installPath + "/../../" + devdir + "/resources");
	// if directory does not exist, we may have an "out-of-source" build and we use the current working directory instead
	if (!resPathInfo.exists())
		resPathInfo = QFileInfo(QDir::currentPath() + "/../../" + devdir + "/resources");
	return resPathInfo.absoluteFilePath();
#endif

#endif // DEPLOYMENT
}


QString Directories::dataDir() {
#ifdef DEPLOYMENT

	// deployment mode, databases are in the same directory as resources
	return resourcesRootDir();

#else // DEPLOYMENT

	// during development, databases are expected in the data directory
	QString installPath = qApp->applicationDirPath();

#if defined(Q_OS_WIN)
	// in development mode, we have the resources in the data directory
	// executables are build in bin/debug or bin/release
	return installPath + "/../../data";
#elif defined(Q_OS_MAC)
	// in development mode, we have the resources outside the bundle
	return installPath + "/../../../../../data";
#elif defined(Q_OS_UNIX)
	return installPath + "/../../data";
#else
#error Implement this for your platform/compiler
#endif

#endif // DEPLOYMENT
}



QString Directories::translationsFilePath(const QString & langID) {
#ifdef BUILD_DEBIAN_PACKAGE
	QString installPath = qApp->applicationDirPath();
	return installPath + QString("/../share/locale/%1/LC_MESSAGES/"+appname+".qm").arg(langID);
#else // BUILD_DEBIAN_PACKAGE
	return resourcesRootDir() + QString("/translations/"+appname+"_%1.qm").arg(langID);
#endif // BUILD_DEBIAN_PACKAGE
}


QString Directories::qtTranslationsFilePath(const QString & langID) {
#if defined(Q_OS_LINUX)
	return QString("/usr/share/qt5/translations/qt_%1.qm").arg(langID);
#else
	// in all other cases the qt_xx.qm files are located in the resources path
	return resourcesRootDir() + QString("/translations/qt_%1.qm").arg(langID);
#endif
}


QString Directories::userDataDir() {
	FUNCID(Directories::userDataDir); (void)FUNC_ID;
	Q_ASSERT_X(!appname.isEmpty(), FUNC_ID,
			   "You must configure Directories::appname and the other static members before "
			   "accessing any static query functions from class Directories!");
	// we have different user data Directories, based on OS
#if defined(Q_OS_WIN)
	// on Windows, we store user data unter %HOME%/AppData/Roaming
	QString fname = QDir::toNativeSeparators(QDir::home().absolutePath() + "/AppData/Roaming/" + appname);
#else
	// on Unix/Mac OS we store user data under home directory
	QString fname = QDir::toNativeSeparators(QDir::home().absolutePath() + "/.local/share/" + appname);
#endif // Q_OS_WIN
	QFileInfo fpath(fname);
	return fpath.absoluteFilePath();
}

QString Directories::homeDir() {
	return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}


QString Directories::tmpDir() {
#if defined(Q_OS_WIN)
	// on Windows, we store user data unter %TMP%
	return QStandardPaths::writableLocation(QStandardPaths::TempLocation);
#else
	// on Unix/Mac OS we use global tmp dir
	return "/tmp";
#endif // Q_OS_WIN
}


QString Directories::globalLogFile() {
	// we should create the parent directory, if it does not yet exist
	QString userDir = userDataDir();
	if (!QFileInfo::exists(userDir))
		QDir().mkpath(userDir);
	return userDir + "/"+appname+".log";
}


QString Directories::generateTempFileName() {
	QString tmpDirectory = tmpDir();

	unsigned int i = 0;
	for (;;) {
		++i;
		// generate file path
		QString fpath = tmpDirectory + QString("/x_tmp%1").arg(i);
		// check if file exists
		if (QFileInfo::exists(fpath))
			continue; // next index
		// check if file can be written into
		if (!checkForWriteAccess(fpath)) {
			qWarning() << QString("Cannot write to temporary file '%1'").arg( fpath );
			break; // no write access to tmp
		}
		return fpath; // file name found
	}

	// cannot write in global tmp, use tmp subdir in user data dir
	tmpDirectory = userDataDir() + "/tmp";
	if (!QDir(tmpDirectory).exists()) {
		if (!QDir().mkdir(tmpDirectory)) {
			qWarning() << QString("Cannot create user-tmp directory '%1'").arg( tmpDirectory );
			return QString();
		}
	}

	for (;;) {
		++i;
		// generate file path
		QString fpath = tmpDirectory + QString("/x_tmp%1").arg(i);
		// check if file exists
		if (QFileInfo::exists(fpath))
			continue; // next index
		// check if file can be written into
		if (!checkForWriteAccess(fpath)) {
			qWarning() << QString("Cannot write to temporary file '%1'").arg( fpath );
			return QString();
		}
		return fpath; // file name found
	}
}


bool Directories::checkForWriteAccess(const QString & newFileName) {
	if (QFile(newFileName).exists())
		return false;
	// check if file can be written into
	QFile f(newFileName);
	if (f.open(QIODevice::WriteOnly)) {
		f.remove();
		return true;
	}
	return false;
}

