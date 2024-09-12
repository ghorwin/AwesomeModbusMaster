/*
*/

#include <AMM_ModbusConnection.h>

#include <IBK_messages.h>
#include <IBK_Exception.h>
#include <IBK_StringUtils.h>
#include <CodeGen_Constants.h>
#include <CodeGen_Utilities.h>

#include <tinyxml.h>

namespace AMM {

void ModbusConnection::readXML(const TiXmlElement * element) {
	FUNCID(ModbusConnection::readXML);

	try {
		// search for mandatory elements
		if (!element->FirstChildElement("Device"))
			throw IBK::Exception( IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
				IBK::FormatString("Missing required 'Device' element.") ), FUNC_ID);

		// reading elements
		const TiXmlElement * c = element->FirstChildElement();
		while (c) {
			const std::string & cName = c->ValueStr();
			if (cName == "ModbusTcp")
				m_modbusTcp = CODEGEN::readPODElement<bool>(c, cName);
			else if (cName == "Device")
				m_device = QString::fromStdString(c->GetText());
			else if (cName == "Baudrate")
				m_baudrate = CODEGEN::readPODElement<int>(c, cName);
			else if (cName == "Parity") {
				std::string val(c->GetText());
				if (val.size() != 1)
					throw IBK::Exception(IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
						IBK::FormatString("Error reading '"+cName+"' tag, expected char.") ), FUNC_ID);
				m_parity = val[0];
			}
			else if (cName == "Databits")
				m_databits = CODEGEN::readPODElement<int>(c, cName);
			else if (cName == "Stopbits")
				m_stopbits = CODEGEN::readPODElement<int>(c, cName);
			else {
				IBK::IBK_Message(IBK::FormatString(XML_READ_UNKNOWN_ELEMENT).arg(cName).arg(c->Row()), IBK::MSG_WARNING, FUNC_ID, IBK::VL_STANDARD);
			}
			c = c->NextSiblingElement();
		}
	}
	catch (IBK::Exception & ex) {
		throw IBK::Exception( ex, IBK::FormatString("Error reading 'ModbusConnection' element."), FUNC_ID);
	}
	catch (std::exception & ex2) {
		throw IBK::Exception( IBK::FormatString("%1\nError reading 'ModbusConnection' element.").arg(ex2.what()), FUNC_ID);
	}
}

TiXmlElement * ModbusConnection::writeXML(TiXmlElement * parent) const {
	TiXmlElement * e = new TiXmlElement("ModbusConnection");
	parent->LinkEndChild(e);

	ModbusConnection DEFAULT;
	if (m_modbusTcp != DEFAULT.m_modbusTcp)
		TiXmlElement::appendSingleAttributeElement(e, "ModbusTcp", nullptr, std::string(), IBK::val2string<bool>(m_modbusTcp));
	if (!m_device.isEmpty())
		TiXmlElement::appendSingleAttributeElement(e, "Device", nullptr, std::string(), m_device.toStdString());
	if (m_baudrate != DEFAULT.m_baudrate)
		TiXmlElement::appendSingleAttributeElement(e, "Baudrate", nullptr, std::string(), IBK::val2string<int>(m_baudrate));
	if (m_parity != DEFAULT.m_parity)
		TiXmlElement::appendSingleAttributeElement(e, "Parity", nullptr, std::string(), std::string(1, m_parity));
	if (m_databits != DEFAULT.m_databits)
		TiXmlElement::appendSingleAttributeElement(e, "Databits", nullptr, std::string(), IBK::val2string<int>(m_databits));
	if (m_stopbits != DEFAULT.m_stopbits)
		TiXmlElement::appendSingleAttributeElement(e, "Stopbits", nullptr, std::string(), IBK::val2string<int>(m_stopbits));
	return e;
}

} // namespace AMM
