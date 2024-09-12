#ifndef AMM_ModbusConnectionH
#define AMM_ModbusConnectionH

#include "CodeGen_Macros.h"

#include <QString>

namespace AMM {

/*! Stores information on modbus connections. */
class ModbusConnection {
public:
	CODEGEN_READWRITE

	/*! If true, modbustcp otherwise  modbusrtu */
	bool			m_modbusTcp = true;		// XML:E:write-if-different
	/*! Device name, i,e. /dev/ttyS1,  192.168.7.100:5000, ...
		When specifying TCP connection, append port as '127.0.0.1:4000'.
	*/
	QString			m_device;				// XML:E:required
	/*! Baudrate (only for serial connection)*/
	int				m_baudrate = 9600;		// XML:E:write-if-different
	/*! Parity 'N','E','O' (no, even, odd) */
	char			m_parity = 'N';			// XML:E:write-if-different
	/*! Databits 7 or 8 */
	int				m_databits = 8;			// XML:E:write-if-different
	/*! Stopbits 1, 2, 3 (for 1.5 stopbits). */
	int				m_stopbits = 1;			// XML:E:write-if-different
};

} // namespace AMM

#endif // AMM_ModbusConnectionH
