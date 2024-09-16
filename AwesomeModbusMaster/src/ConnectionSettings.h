#ifndef CONNECTIONSETTINGS_H
#define CONNECTIONSETTINGS_H

#include <QWidget>

namespace Ui {
	class ConnectionSettings;
}

namespace AMM {
	class ModbusConnection;
}

/*! A widget for entering connection settings.
	Used in logging/simulation tab.
*/
class ConnectionSettings : public QWidget {
	Q_OBJECT

public:
	explicit ConnectionSettings(QWidget *parent = nullptr);
	~ConnectionSettings() override;

	/*! Transfers data from stored connection settings into widget. */
	void updateUi(const AMM::ModbusConnection & connectionSettings);

	/*! Returns currently entered connection settings. */
	void currentSettings(AMM::ModbusConnection & connectionSettings) const;

signals:
	/*! Emitted, when user has made any changes to the settings. */
	void settingsChanged();

	/*! Emitted, when user has pushed the connection button, either to
		connect or to sever connection.
	*/
	void connectionButtonToggled();

private slots:
	void on_radioButtonModbusTCP_toggled(bool checked);

private:
	Ui::ConnectionSettings *m_ui;
};

#endif // CONNECTIONSETTINGS_H
