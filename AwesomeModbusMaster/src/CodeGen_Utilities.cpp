// Code Generator Utilities, based on NANDRAD_Utilities.h from SIM-VICUS Project
#include "CodeGen_Utilities.h"

#include <tinyxml.h>

#include <IBK_StringUtils.h>
#include <IBK_FileUtils.h>
#include <IBK_Path.h>
#include <IBK_Flag.h>
#include <IBK_Unit.h>
#include <IBK_Parameter.h>
#include <IBK_IntPara.h>

namespace CODEGEN {

TiXmlElement * openXMLFile(const std::map<std::string,IBK::Path> & pathPlaceHolders, const IBK::Path & filename,
	const std::string & parentXmlTag, TiXmlDocument & doc)
{
	FUNCID(CG::openXMLFile);
	// replace path placeholders
	IBK::Path fname = filename.withReplacedPlaceholders( pathPlaceHolders );

	if ( !fname.isFile() )
		throw IBK::Exception(IBK::FormatString("File '%1' does not exist or cannot be opened for reading.")
				.arg(fname), FUNC_ID);

	if (!doc.LoadFile(fname.str().c_str(), TIXML_ENCODING_UTF8)) {
		throw IBK::Exception(IBK::FormatString("Error in line %1 of project file '%2':\n%3")
				.arg(doc.ErrorRow())
				.arg(filename)
				.arg(doc.ErrorDesc()), FUNC_ID);
	}

	// we use a handle so that NULL pointer checks are done during the query functions
	TiXmlHandle xmlHandleDoc(&doc);

	// read root element
	TiXmlElement * xmlElem = xmlHandleDoc.FirstChildElement().Element();
	if (!xmlElem)
		return nullptr; // empty file?
	std::string rootnode = xmlElem->Value();
	if (rootnode != parentXmlTag)
		throw IBK::Exception( IBK::FormatString("Expected '%1' as root node in XML file.").arg(parentXmlTag), FUNC_ID);

	return xmlElem;
}



void readParameterElement(const TiXmlElement * element, IBK::Parameter & p) {
	FUNCID(CG::readParameterElement);
	const TiXmlAttribute* attrib = TiXmlAttribute::attributeByName(element, "name");
	if (attrib == nullptr)
		throw IBK::Exception(IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg("Missing 'name' attribute in IBK:Parameter element."), FUNC_ID);
	const std::string & namestr = attrib->ValueStr();
	attrib = TiXmlAttribute::attributeByName(element, "unit");
	if (attrib == nullptr)
		throw IBK::Exception(IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg("Missing 'unit' attribute in IBK:Parameter element."), FUNC_ID);
	const std::string & unit = attrib->ValueStr();
	IBK::Unit u;
	try {
		u.set(unit);
	} catch (...) {
		throw IBK::Exception(IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg("Unknown/invalid unit '"+unit+"' in IBK:Parameter element."), FUNC_ID);
	}
	const char * const str = element->GetText();
	try {
		double val = IBK::string2val<double>(str);
		p.set(namestr, val, u);
	} catch (...) {
		throw IBK::Exception(IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg("Cannot read value in IBK:Parameter element."), FUNC_ID);
	}
}


void readIntParaElement(const TiXmlElement * element, IBK::IntPara & p) {
	FUNCID(CG::readIntParaElement);
	const TiXmlAttribute* attrib = TiXmlAttribute::attributeByName(element, "name");
	if (attrib == nullptr)
		throw IBK::Exception(IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg("Missing 'name' attribute in IBK:IntPara element."), FUNC_ID);
	const std::string & namestr = attrib->ValueStr();
	const char * const str = element->GetText();
	try {
		int val = IBK::string2val<int>(str);
		p.set(namestr, val);
	} catch (...) {
		throw IBK::Exception(IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg("Cannot read value in IBK:IntPara element."), FUNC_ID);
	}
}


void readFlagElement(const TiXmlElement * element, IBK::Flag & f) {
	FUNCID(CG::readFlagElement);
	std::string namestr;
	std::string valueStr;
	TiXmlElement::readSingleAttributeElement(element, "name", namestr, valueStr);
	if (valueStr == "true" || valueStr == "1")
		f.set(namestr, true);
	else if (valueStr == "false" || valueStr == "0")
		f.set(namestr, false);
	else {
		throw IBK::Exception( IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
			IBK::FormatString("Error reading 'IBK:Flag' tag, expected 'true' or 'false' as value.") ), FUNC_ID);
	}
}


IBK::Unit readUnitElement(const TiXmlElement * element, const std::string & eName) {
	FUNCID(CG::readUnitElement);
	try {
		return IBK::Unit(element->GetText());
	} catch (IBK::Exception & ex) {
		throw IBK::Exception( ex, IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
			IBK::FormatString("Error reading '"+eName+"' tag, invalid/unknown unit'"+element->GetText()+"'.") ), FUNC_ID);
	}
}


IBK::Time readTimeElement(const TiXmlElement * element, const std::string & eName) {
	FUNCID(CG::readTimeElement);
	IBK::Time t = IBK::Time::fromDateTimeFormat(element->GetText());
	if (!t.isValid())
		throw IBK::Exception( IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
			IBK::FormatString("Error reading '"+eName+"' tag, invalid date/time format '"+element->GetText()+"', expected 'dd.MM.yyyy hh:mm:ss'.") ), FUNC_ID);
	return t;
}


template<>
void readVector<double>(const TiXmlElement * element, const std::string & name, std::vector<double> & vec) {
	FUNCID(CG::readVector);
	std::string text = element->GetText();
	text = IBK::replace_string(text, ",", " ");
	try {
		IBK::string2valueVector(text, vec);
	} catch (IBK::Exception & ex) {
		throw IBK::Exception( ex, IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
			IBK::FormatString("Error reading vector element '%1'.").arg(name) ), FUNC_ID);
	}
}


template<>
void readVector(const TiXmlElement * element, const std::string & /*name*/, std::vector<std::string> & vec) {
	std::string text = element->GetText();
	IBK::explode(text, vec, ",", IBK::EF_KeepEmptyTokens | IBK::EF_UseQuotes);
}


void readVectorQuotedString(const TiXmlElement * element, const std::string & /*name*/, std::vector<std::string> & vec) {
	std::string text = element->GetText();
	IBK::explode(text, vec, ",", IBK::EF_KeepEmptyTokens);
}


template<>
void readPoint2D<double>(const TiXmlElement * element, const std::string & name, IBK::point2D<double> & p) {
	FUNCID(CG::readVector);
	std::string text = element->GetText();
	try {
		std::vector<double> vec;
		IBK::string2valueVector(text, vec);
		if (vec.size() != 2)
			throw IBK::Exception("Size mismatch, expected 2 numbers separated by , .", FUNC_ID);
		p.m_x = vec[0];
		p.m_y = vec[1];
	} catch (IBK::Exception & ex) {
		throw IBK::Exception( ex, IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
			IBK::FormatString("Error reading point2D element '%1'.").arg(name) ), FUNC_ID);
	}
}


template <>
void writeVector(TiXmlElement * parent, const std::string & name, const std::vector<std::string> & vec) {
	if (!vec.empty()) {
		TiXmlElement * child = new TiXmlElement(name);
		parent->LinkEndChild(child);

		std::stringstream vals;
		for (unsigned int i=0; i<vec.size(); ++i) {
			vals << '"' << vec[i] << '"';
			if (i<vec.size()-1)  vals << ",";
		}
		TiXmlText * text = new TiXmlText( vals.str() );
		child->LinkEndChild( text );
	}
}





} // namespace CODEGEN
