#include "SimulationView.h"
#include "ui_SimulationView.h"

SimulationView::SimulationView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::SimulationView)
{
	ui->setupUi(this);
}

SimulationView::~SimulationView()
{
	delete ui;
}
