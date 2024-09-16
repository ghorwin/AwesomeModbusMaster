#include "ReadWriteView.h"
#include "ui_ReadWriteView.h"

ReadWriteView::ReadWriteView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ReadWriteView)
{
	ui->setupUi(this);
}

ReadWriteView::~ReadWriteView()
{
	delete ui;
}
