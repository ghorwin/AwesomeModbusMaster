/*
*/

#include <AMM_ModbusConfig.h>

#include <IBK_messages.h>
#include <IBK_Exception.h>
#include <IBK_StringUtils.h>
#include <CodeGen_Constants.h>
#include <AMM_KeywordList.h>
#include <CodeGen_Utilities.h>

#include <tinyxml.h>

namespace AMM {

void ModbusConfig::readXML(const TiXmlElement * element) {
	FUNCID(ModbusConfig::readXML);

	try {
		// search for mandatory attributes
		if (!TiXmlAttribute::attributeByName(element, "id"))
			throw IBK::Exception( IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
				IBK::FormatString("Missing required 'id' attribute.") ), FUNC_ID);

		// reading attributes
		const TiXmlAttribute * attrib = element->FirstAttribute();
		while (attrib) {
			const std::string & attribName = attrib->NameStr();
			if (attribName == "id")
				m_id = (IDType)CODEGEN::readPODAttributeValue<unsigned int>(element, attrib);
			else {
				IBK::IBK_Message(IBK::FormatString(XML_READ_UNKNOWN_ATTRIBUTE).arg(attribName).arg(element->Row()), IBK::MSG_WARNING, FUNC_ID, IBK::VL_STANDARD);
			}
			attrib = attrib->Next();
		}
		// search for mandatory elements
		if (!element->FirstChildElement("SlaveID"))
			throw IBK::Exception( IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
				IBK::FormatString("Missing required 'SlaveID' element.") ), FUNC_ID);

		if (!element->FirstChildElement("FunctionCode"))
			throw IBK::Exception( IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
				IBK::FormatString("Missing required 'FunctionCode' element.") ), FUNC_ID);

		if (!element->FirstChildElement("Register"))
			throw IBK::Exception( IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
				IBK::FormatString("Missing required 'Register' element.") ), FUNC_ID);

		if (!element->FirstChildElement("DataType"))
			throw IBK::Exception( IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
				IBK::FormatString("Missing required 'DataType' element.") ), FUNC_ID);

		// reading elements
		const TiXmlElement * c = element->FirstChildElement();
		while (c) {
			const std::string & cName = c->ValueStr();
			if (cName == "Description")
				m_description = QString::fromStdString(c->GetText());
			else if (cName == "SlaveID")
				m_slaveID = (IDType)CODEGEN::readPODElement<unsigned int>(c, cName);
			else if (cName == "FunctionCode")
				m_functionCode = CODEGEN::readPODElement<unsigned int>(c, cName);
			else if (cName == "Register")
				m_register = CODEGEN::readPODElement<unsigned int>(c, cName);
			else if (cName == "ValueCount")
				m_valueCount = CODEGEN::readPODElement<unsigned int>(c, cName);
			else if (cName == "Descriptions") {
				std::vector<std::string> tmpList;
				CODEGEN::readVector(c, "Descriptions", tmpList);
				m_descriptions.clear();
				for (const std::string & s : tmpList)
					m_descriptions.append(QString::fromStdString(s) );
			}
			else if (cName == "ReadFlag")
				m_readFlag = CODEGEN::readPODElement<bool>(c, cName);
			else if (cName == "WriteFlag")
				m_writeFlag = CODEGEN::readPODElement<bool>(c, cName);
			else if (cName == "Interval")
				m_interval = CODEGEN::readPODElement<unsigned int>(c, cName);
			else if (cName == "DataType") {
				try {
					m_dataType = (DataType)KeywordList::Enumeration("ModbusConfig::DataType", c->GetText());
				}
				catch (IBK::Exception & ex) {
					throw IBK::Exception( ex, IBK::FormatString(XML_READ_ERROR).arg(c->Row()).arg(
						IBK::FormatString("Invalid or unknown keyword '"+std::string(c->GetText())+"'.") ), FUNC_ID);
				}
			}
			else {
				IBK::IBK_Message(IBK::FormatString(XML_READ_UNKNOWN_ELEMENT).arg(cName).arg(c->Row()), IBK::MSG_WARNING, FUNC_ID, IBK::VL_STANDARD);
			}
			c = c->NextSiblingElement();
		}
	}
	catch (IBK::Exception & ex) {
		throw IBK::Exception( ex, IBK::FormatString("Error reading 'ModbusConfig' element."), FUNC_ID);
	}
	catch (std::exception & ex2) {
		throw IBK::Exception( IBK::FormatString("%1\nError reading 'ModbusConfig' element.").arg(ex2.what()), FUNC_ID);
	}
}

TiXmlElement * ModbusConfig::writeXML(TiXmlElement * parent) const {
	TiXmlElement * e = new TiXmlElement("ModbusConfig");
	parent->LinkEndChild(e);

	if (m_id != CG::INVALID_ID)
		e->SetAttribute("id", IBK::val2string<IDType>(m_id));
	ModbusConfig DEFAULT;
	if (!m_description.isEmpty())
		TiXmlElement::appendSingleAttributeElement(e, "Description", nullptr, std::string(), m_description.toStdString());
	if (m_slaveID != CG::INVALID_ID)
			TiXmlElement::appendSingleAttributeElement(e, "SlaveID", nullptr, std::string(), IBK::val2string<unsigned int>(m_slaveID));
	TiXmlElement::appendSingleAttributeElement(e, "FunctionCode", nullptr, std::string(), IBK::val2string<unsigned int>(m_functionCode));
	TiXmlElement::appendSingleAttributeElement(e, "Register", nullptr, std::string(), IBK::val2string<unsigned int>(m_register));

	if (m_dataType != NUM_DT)
		TiXmlElement::appendSingleAttributeElement(e, "DataType", nullptr, std::string(), KeywordList::Keyword("ModbusConfig::DataType",  m_dataType));
	if (m_valueCount != DEFAULT.m_valueCount)
		TiXmlElement::appendSingleAttributeElement(e, "ValueCount", nullptr, std::string(), IBK::val2string<unsigned int>(m_valueCount));
	if (m_descriptions != DEFAULT.m_descriptions)
		if (!m_descriptions.isEmpty()) {
		std::vector<std::string> tmpList;
		for (const QString & s : m_descriptions)
			tmpList.push_back(s.toStdString());
		CODEGEN::writeVector(e, "Descriptions", tmpList);
	}
	if (m_readFlag != DEFAULT.m_readFlag)
		TiXmlElement::appendSingleAttributeElement(e, "ReadFlag", nullptr, std::string(), IBK::val2string<bool>(m_readFlag));
	if (m_writeFlag != DEFAULT.m_writeFlag)
		TiXmlElement::appendSingleAttributeElement(e, "WriteFlag", nullptr, std::string(), IBK::val2string<bool>(m_writeFlag));
	if (m_interval != DEFAULT.m_interval)
		TiXmlElement::appendSingleAttributeElement(e, "Interval", nullptr, std::string(), IBK::val2string<unsigned int>(m_interval));
	return e;
}

} // namespace AMM
