#ifndef DebugApplicationH
#define DebugApplicationH

#include <QApplication>

class QWidget;

/*! This class catches all exceptions thrown during eventloop execution.
	It basically programmed for debug purposes.
*/
class DebugApplication : public QApplication {
	Q_OBJECT
public:
	/*! ctor relay. */
	DebugApplication( int & argc, char ** argv ) :
		QApplication( argc, argv )
	{
	}

	/*! We just reimplement QApplication::notify() to catch all exceptions and allow setting a breakpoint here. */
	bool notify( QObject *recv, QEvent *e ) override;
};


#endif // DebugApplicationH
