#include "DebugApplication.h"

#include <QDebug>
#include <QLoggingCategory>
Q_LOGGING_CATEGORY(lcDebugApplication, "AMM.DebugApplication");

#include "Exception.h"

#include "MainWindow.h"

bool DebugApplication::notify( QObject *recv, QEvent *e ) {
	try {

		return QApplication::notify( recv, e );
	}
	catch (Exception &ex) {
		ex.writeMsgStackToError();
		qCCritical(lcDebugApplication) << "Exception caught!";
	}
	catch (std::exception &ex) {
		qCCritical(lcDebugApplication).noquote().nospace() << "Exception caught: " << ex.what();
	}

	return false;
}

