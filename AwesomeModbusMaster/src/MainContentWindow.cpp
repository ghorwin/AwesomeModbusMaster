#include "MainContentWindow.h"
#include "ui_MainContentWindow.h"

#include "ScanView.h"
#include "ReadWriteView.h"
#include "LoggingView.h"
#include "SimulationView.h"

MainContentWindow::MainContentWindow(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::MainContentWindow)
{
	m_ui->setupUi(this);
	m_ui->verticalLayout->setMargin(0);

	m_scanView  = new ScanView(this);
	m_loggingView = new LoggingView(this);
	m_readWriteView = new ReadWriteView(this);
	m_simulationView = new SimulationView(this);

	m_ui->tabWidget->addTab(m_scanView, tr("Scan"));
	m_ui->tabWidget->addTab(m_readWriteView, tr("Read/Write"));
	m_ui->tabWidget->addTab(m_loggingView, tr("Logging"));
	m_ui->tabWidget->addTab(m_simulationView, tr("Simulation"));
}


MainContentWindow::~MainContentWindow() {
	delete m_ui;
}

