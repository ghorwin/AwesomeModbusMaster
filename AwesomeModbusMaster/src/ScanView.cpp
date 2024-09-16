#include "ScanView.h"
#include "ui_ScanView.h"

ScanView::ScanView(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::ScanView)
{
	m_ui->setupUi(this);
}

ScanView::~ScanView()
{
	delete m_ui;
}
