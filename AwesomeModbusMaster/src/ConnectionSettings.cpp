#include "ConnectionSettings.h"
#include "ui_ConnectionSettings.h"

#include "AMM_ModbusConnection.h"

ConnectionSettings::ConnectionSettings(QWidget *parent) :
	QWidget(parent),
	m_ui(new Ui::ConnectionSettings)
{
	m_ui->setupUi(this);

	m_ui->gridLayoutTCP->setMargin(0);
	m_ui->gridLayoutRTU->setMargin(0);
	on_radioButtonModbusTCP_toggled(true);
}


ConnectionSettings::~ConnectionSettings() {
	delete m_ui;
}


void ConnectionSettings::updateUi(const AMM::ModbusConnection & connectionSettings) {

}


void ConnectionSettings::currentSettings(AMM::ModbusConnection & connectionSettings) const {
	connectionSettings.m_modbusTcp = m_ui->radioButtonModbusTCP->isChecked();
	if (connectionSettings.m_modbusTcp) {
		connectionSettings.m_device = m_ui->lineEditHostname->text().trimmed() + ":" + m_ui->lineEditPort->text().trimmed();
	}
	else {
		connectionSettings.m_device = m_ui->lineEditCOM->text().trimmed();
//		connectionSettings.m_

	}

}


void ConnectionSettings::on_radioButtonModbusTCP_toggled(bool checked) {
	m_ui->widgetInputRTU->setVisible(!checked);
	m_ui->widgetInputTCP->setVisible(checked);
}

