#include "LoggingView.h"
#include "ui_LoggingView.h"

LoggingView::LoggingView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LoggingView)
{
	ui->setupUi(this);
}

LoggingView::~LoggingView()
{
	delete ui;
}
