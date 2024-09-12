#include "AMM_ModbusConfig.h"

#include <QtCore/QLoggingCategory>
Q_LOGGING_CATEGORY(lcBtpModbusConfig, "aww.modbusconfig")

#include "AMM_KeywordList.h"
#include "Exception.h"

namespace AMM {

template <class Key, class T> class InitializableQMap : public QMap<Key,T>
{
public:
	inline InitializableQMap<Key,T> &operator<< (const QPair<Key,T> &t)
	{ this->insert(t.first,t.second); return *this; }
};


void ModbusConfig::evalDataSize() {
	FUNCID(ModbusConfig::evalDataSize);

	// Stores number of byte per data type
	static const InitializableQMap<ModbusConfig::DataType,int> KNOWN_TYPES = InitializableQMap<ModbusConfig::DataType,int>()
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_bit,				1)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_integer8,			1)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_integer16_ab,		1)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_integer16_ba,		1)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_long32_abcd,		2)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_long32_badc,		2)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_long32_cdab,		2)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_long32_dcba,		2)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_integer64_abcd,	4)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_float32_abcd,		2)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_float32_badc,		2)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_float32_cdab,		2)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_float32_dcba,		2)
		<< QPair<ModbusConfig::DataType,int>(ModbusConfig::DT_char,				1)
	;

	QMap<ModbusConfig::DataType,int>::const_iterator it = KNOWN_TYPES.find(m_dataType);
	if (it == KNOWN_TYPES.end())
		throw Exception(QString("Unknown modbus register data type %1 for modbus register %2.")
							 .arg(KeywordList::Keyword("ModbusConfig::DataType", m_dataType)).arg(m_id), FUNC_ID);

	// check valid value count
	if (m_valueCount == 0)
		throw Exception(QString("Invalid value count for modbus config #%1").arg(m_id), FUNC_ID);

	// determine number of bytes for selected data type
	m_dataSize = m_valueCount*it.value();
}

} // namespace AMM
