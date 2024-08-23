#ifndef AMM_ExceptionH
#define AMM_ExceptionH

#include <QString>

#include <stdexcept>
#include <list>

#include "Constants.h"

/*! Wrapper class that stores not only exception message, but also location and allows
	collection of exception stacks.

	This is the general exception class used when exceptions are thrown from anywhere in the code.
	Use this class instead of the standard library exception classes.

	When throwing an exception from within a function, pass a message text and an information about
	the location. The following code shows an example for an exception throw from within a function
	splineCheck().
	\code
	FUNCID(functions::splineCheck); // generate a uniform function signature
	if (spline.empty())
		throw Exception("Spline must not be empty!", FUNC_ID);
	\endcode

	If you catch and re-throw an exception, also pass the old exception to build up the exception stack.
	\code
	try {
		// some code
		splineCheck();
	}
	catch (const Exception & ex) {
		throw Exception(ex, "Error initializing module!", FUNC_ID);
	}
	\endcode

	You can access the exception list by using the member function msgs() and process
	the individual messages stored in the MsgInfo struct.
*/
class Exception : public std::exception {
public:
	/*! Struct containing message info. */
	struct MsgInfo {
		/*! Convenience constructor, creates a message info struct.
			\param	w	Error message text.
			\param	loc Location string of the routine throwing the exception.
		*/
		MsgInfo(const QString & w, const QString & loc) : what(w), location(loc) {}
		QString	what;		///< The message text.
		QString	location;	///< The location.
	};

	/*! Destructor declaration needed because std::~exception() has throw specifier. */
	~Exception() {}

	/*! Constructor for creating a new exception.
		\param	what	Error message text.
		\param	loc		Location string of the routine throwing the exception.
	*/
	Exception(const QString& what, const QString& loc);

	/*! Constructor for re-throwing an exception.
		\param	old		Previously thrown exception that currently was caught.
		\param	what	Additional error message text.
		\param	loc		Location string of the routine throwing the exception.
	*/
	Exception(const Exception & old, const QString& what, const QString& loc);

	/*! Constructor for re-throwing an exception with given message stack.
		\param	old		Previously thrown exception that currently was caught.
		\param	what	Additional error message text.
		\param	loc		Location string of the routine throwing the exception.
	*/
	Exception(const std::list<MsgInfo> & msgs, const QString& what, const QString& loc);

	/*! Accesses the top-most (last) exception message.
		\return Error message string.

		\warning This function is not thread-safe!
	*/
	const char* what() 	const noexcept;

	/*! Accesses the top-most (last) exception location.
		\return Location string.
	*/
	QString location() const;

	/*! Accesses the stack of messages from all caught exceptions.
		\return Current message stack.
	*/
	const std::list<MsgInfo> & msgs() const { return m_msgs; }

	/*! Writes the content of the message list as qCritical() to the message handler. */
	void writeMsgStackToError() const;

private:
	/*! The message list. */
	std::list<MsgInfo> m_msgs;
};


#endif // AMM_ExceptionH
