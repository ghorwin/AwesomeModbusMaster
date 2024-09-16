#ifndef MAINCONTENTWINDOW_H
#define MAINCONTENTWINDOW_H

#include <QWidget>

namespace Ui {
class MainContentWindow;
}

class ScanView;
class ReadWriteView;
class LoggingView;
class SimulationView;

/*! Holds the actual content of the application. */
class MainContentWindow : public QWidget {
	Q_OBJECT

public:
	explicit MainContentWindow(QWidget *parent = nullptr);
	~MainContentWindow() override;

private:
	Ui::MainContentWindow *m_ui;

	ScanView				*m_scanView			= nullptr;
	ReadWriteView			*m_readWriteView	= nullptr;
	LoggingView				*m_loggingView		= nullptr;
	SimulationView			*m_simulationView	= nullptr;
};

#endif // MAINCONTENTWINDOW_H
