#ifndef UndoCommandBaseH
#define UndoCommandBaseH

#include <QUndoCommand>
#include <QCoreApplication>

namespace AMM {
	class Config;
}

/*! Abstract base class for all modification actions. */
class ModificationInfo {
public:
	virtual ~ModificationInfo();
};

/*! Abstract base class for all undo commands.
	It provides the member function push() which puts the command to the global stack.
	Also, via theProject() read/write access to the project data is granted.
*/
class UndoCommandBase : public QUndoCommand {
	Q_DECLARE_TR_FUNCTIONS(UndoCommands)
public:
	/*! Pushes the command to the global undo-stack (in the main window). */
	void push();

protected:
	/*! Returns a read/write reference to the project data. */
	AMM::Config&	theProject() const;
};


#endif // UndoCommandBaseH
