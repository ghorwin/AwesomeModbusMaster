#ifndef AMM_ModbusConfigH
#define AMM_ModbusConfigH

#include <QString>
#include <QMap>
#include <QPair>

#include "CodeGen_Macros.h"
#include "CodeGen_Constants.h"

namespace AMM {

class ModbusConnection;

/*! Stores information on modbus register read/write calls. */
class ModbusConfig {
public:
	CODEGEN_READWRITE

	/*! Supported data types. */
	enum DataType {
		DT_bit,					// Keyword: bit
		DT_char,				// Keyword: char
		DT_integer8,			// Keyword: integer8
		DT_integer16_ab,		// Keyword: int16_ab
		DT_integer16_ba,		// Keyword: int16_ba
		DT_long32_cdab,			// Keyword: long32_cdab
		DT_long32_abcd,			// Keyword: long32_abcd
		DT_long32_dcba,			// Keyword: long32_dcba
		DT_long32_badc,			// Keyword: long32_badc
		DT_integer64_abcd,		// Keyword: integer64_abcd
		DT_float32_cdab,		// Keyword: float32_cdab
		DT_float32_abcd,		// Keyword: float32_abcd
		DT_float32_dcba,		// Keyword: float32_dcba
		DT_float32_badc,		// Keyword: float32_badc
		NUM_DT
	};

	/*! Determine based on m_dataType the size of the register in words. */
	void evalDataSize();

	/*! Unique ID of this modbus config entry (corresponds to value ID, mandatory).
		Value is usually obtained without calibration/conversion.
		May be grouped with numbering condition (modbus.adoc).
	*/
	IDType			m_id;							// XML:A:required
	/*! Description */
	QString			m_description;					// XML:E

	/*! Slave ID, needed for modbus request (for each device), follows ID numbering scheme, see
		documentation (modbus.adoc).
	*/
	IDType			m_slaveID = CG::INVALID_ID;		// XML:E:required
	/*! Modbus function code, see libmodus */
	unsigned int	m_functionCode;					// XML:E:required
	/*! Register number. */
	unsigned int	m_register;						// XML:E:required
	/*! Data type (typename as string): see ENUM with keywordlist */
	DataType		m_dataType;						// XML:E:required
	/*! Number of values for vector quantities, by default 1 (single value), must be >= 1. */
	unsigned int	m_valueCount = 1;				// XML:E:write-if-different

	/*! Descriptions of the values, must be same number as valueCount. */
	QStringList		m_descriptions;					// XML:E:write-if-different

	/*! Can read? */
	bool			m_readFlag			= true;		// XML:E:write-if-different
	/*! Can write? */
	bool			m_writeFlag			= false;	// XML:E:write-if-different

	/*! Interval, minimum time that needs to pass until this register is polled again in [ms].
		If = 0, the default polling interval is used.
	*/
	unsigned int	m_interval = 0;					// XML:E:write-if-different

	// runtime-variables only

	/*! Pointer to device connection, set after reading and checking config. */
	const ModbusConnection * m_modbusConnection = nullptr;

	/*! Data type size, automatically calculated from dataType and valueCount  in evalDataSize(). */
	unsigned int	m_dataSize;
};

} // namespace BTP

#endif // ModbusConfigH
