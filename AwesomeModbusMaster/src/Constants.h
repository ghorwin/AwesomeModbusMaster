#ifndef ConstantsH
#define ConstantsH

/*! To be used as institution/organization in Settings. */
extern const char * const ORG_NAME;
/*! Program name, also used as registry/settings key for saving the user
	defined settings and the MRU list.
	To be used as application name in Settings.
*/
extern const char * const PROGRAM_NAME;

/*! Version number of the data model and project file (major.minor). */
extern const char * const VERSION;
/*! Long version number of the data model and project file (major.minor.patch). */
extern const char * const LONG_VERSION;

/*! Manual URL. */
extern const char * const MANUAL_URL;
extern const char * const NEWS_FILE_URL;

/*! A define that generates a consistent function signature for debugging/logger outputs.

	Usage:
	\code
		void MyClass::myFunction() {
			FUNCID(MyClass::myFunction);

			if (spline.empty())
				throw Exception("Spline must not be empty!", FUNC_ID);
		}
	\endcode
*/
#define FUNCID(x) const char * const FUNC_ID = "[" #x "]"


#endif  // ConstantsH
