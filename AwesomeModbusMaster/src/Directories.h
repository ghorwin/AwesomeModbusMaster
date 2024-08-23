/*! This file is based on code from https://github.com/ghorwin/MasterSim (LGPL Licensed). */

#ifndef DirectoriesH
#define DirectoriesH

#include <QString>

/*! Provides default locations for resource and user data files based on DEPLOYMENT
	flag setting and OS.

	Use this as base class for application-specific directory management.

	\note A specific directory layout within the source code directory is required!

	During development:

	\code
	/bin/debug/MasterSim.exe     <- install dir during development - debug mode
	/bin/release/MasterSim.exe   <- install dir during development - release mode
	/data                        <- auxilliary data files
	/MasterSim/resources/translations
	/MasterSim/src/...

	\endcode


	During deployment:

	\code
	/MasterSim.exe               <- install dir during deployment
	/resources/translations

	\endcode

*/
class Directories {
public:
	/*! This directory name is appended to all platform-specific locations.
		For example 'MasterSim' will generate a database save location: /home/<user>/.local/share/MasterSim
		or C:\Users\<user>\AppData\Roaming\MasterSim on Windows.
	*/
	static QString appname;
	/*! The top-level directory within the source code directory tree, where resources and translations can be found.
		For example 'MasterSim' will be used to generate the translation dir: `resourcesRootDir() + "/../MasterSim/resources/translations"`
	*/
	static QString devdir;
	/*! Name of the Linux/Unix package, determines install location of resources.
		For example 'mastersim' will generate a resource: /usr/share/mastersim
	*/
	static QString packagename;

	// *** STATIC ACCESS FUNCTIONS ***

	/*! Returns the platform-specific root directory of all read-only resource files. */
	static QString resourcesRootDir();

	/*! Returns the platform-specific directory of all read-only data files. */
	static QString dataDir();

	/*! Returns the path to the application's translation file path. */
	static QString translationsFilePath(const QString & langID);

	/*! Returns the path to the Qt translation file path. */
	static QString qtTranslationsFilePath(const QString & langID);

	/*! Returns the platform-specific root directory of all user database files. */
	static QString userDataDir();

	/*! Returns the platform-specific root directory of all user database files. */
	static QString homeDir();

	/*! Returns the platform-specific temporary directory. */
	static QString tmpDir();

	/*! Returns path to global log file. */
	static QString globalLogFile();

	/*! Generates a new temporary file path (file not yet existing) and tests writing into this file.
		If writing into file is not possible, an empty string is returned.
	*/
	static QString generateTempFileName();

	/*! This function opens the file in "write" mode and removes the file afterwards again to test if writing is possible.
		\return True, if file can be written into. False if the file already exists, or if file cannot be opened for writing.

		\note Function expects parent directory to exist.
	*/
	static bool checkForWriteAccess(const QString & newFileName);
};

#endif // DirectoriesH
