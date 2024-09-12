#ifndef AMM_ConfigH
#define AMM_ConfigH

#include <CodeGen_Macros.h>
#include <QString>
#include <vector>

#include "AMM_ModbusConnection.h"
#include "AMM_ModbusConfig.h"

namespace AMM {

/*! Contains all data for a Modbus logging configuration. */
class Config {
	CODEGEN_READWRITE_PRIVATE
public:

	// *** PUBLIC MEMBER FUNCTIONS ***

	/*! Reads the project data from an XML file.
		\param filename  The full path to the project file.
	*/
	void readXML(const QString & filename);

	/*! Writes the project file to an XML file.
		\param filename  The full path to the project file.
	*/
	void writeXML(const QString & filename) const;


	/*! Interface parameters */
	ModbusConnection	m_modbusConnection;
};


} // AMM

#endif // AMM_ConfigH
