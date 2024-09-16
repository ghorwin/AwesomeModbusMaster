#include "MainContentWindow.h"
#include "ui_MainContentWindow.h"

MainContentWindow::MainContentWindow(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::MainContentWindow)
{
	m_ui->setupUi(this);
}


MainContentWindow::~MainContentWindow() {
	delete m_ui;
}

