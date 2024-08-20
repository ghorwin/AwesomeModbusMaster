// Code Generator Utilities, based on NANDRAD_Utilities.h from SIM-VICUS Project
#ifndef CodeGen_UtilitiesH
#define CodeGen_UtilitiesH

#include <string>
#include <map>

#include <tinyxml.h>

#include "CodeGen_Constants.h"

#include <IBK_Path.h>
#include <IBK_Time.h>
#include <IBK_LinearSpline.h>
#include <IBK_StringUtils.h>
#include <IBK_point.h>

namespace IBK {
	class Unit;
	class Parameter;
	class IntPara;
	class Flag;
}

class TiXmlDocument;
class TiXmlElement;

namespace CODEGEN {

/*! Attempts to open an XML file, hereby substituting placeholders in the file name and checking if
	the top-level XML tag matches the requested tag name.
*/
TiXmlElement * openXMLFile(const std::map<std::string,IBK::Path>  &pathPlaceHolders, const IBK::Path & filename,
	const std::string & parentXmlTag, TiXmlDocument & doc);

/*! Reads a linear spline from XML element (with proper error handling). */
void readLinearSplineElement(const TiXmlElement * element,
							 IBK::LinearSpline & spl, std::string & name, IBK::Unit * xunit, IBK::Unit * yunit);

/*! Writes a linear spline into XML format.
	\code
	<IBK:LinearSpline name="MySpline">
		<X unit="m">0 1 1.4 2 </X>
		<Y unit="C">1 2 3.4 5 </Y>
	</IBK:LinearSpline>
	\endcode
*/
void writeLinearSplineElement(TiXmlElement * parent, const std::string & name, const IBK::LinearSpline & spl, const std::string & xunit, const std::string & yunit);

/*! Reads an IBK::Parameter from XML element (with proper error handling). */
void readParameterElement(const TiXmlElement * element, IBK::Parameter & p);

/*! Reads an IBK::IntPara from XML element (with proper error handling). */
void readIntParaElement(const TiXmlElement * element, IBK::IntPara & p);

/*! Reads an IBK::Flag from XML element (with proper error handling). */
void readFlagElement(const TiXmlElement * element, IBK::Flag & f);

template <typename T>
void readVector(const TiXmlElement * element, const std::string & name, std::vector<T> & vec) {
	FUNCID(CG::readVector);
	std::string text = element->GetText();
	text = IBK::replace_string(text, ",", " ");
	try {
		IBK::string2vector(text, vec);
	} catch (IBK::Exception & ex) {
		throw IBK::Exception( ex, IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
			IBK::FormatString("Error reading vector element '%1'.").arg(name) ), FUNC_ID);
	}
}

/*! Special implementation for double-type vectors. */
template <>
void readVector(const TiXmlElement * element, const std::string & name, std::vector<double> & vec);

/*! Special implementation for std::string-type vectors.
	Hereby, values are comma-separated, without quotes.
*/
template <>
void readVector(const TiXmlElement * element, const std::string & name, std::vector<std::string> & vec);

/*! Special implementation for std::string-type vectors.
	Hereby, values are comma-separated, quoted strings (escaped via &quot;).
*/
void readVectorQuotedString(const TiXmlElement * element, const std::string & name, std::vector<std::string> & vec);

template <typename T>
void writeVector(TiXmlElement * parent, const std::string & name, const std::vector<T> & vec) {
	if (!vec.empty()) {
		TiXmlElement * child = new TiXmlElement(name);
		parent->LinkEndChild(child);

		std::stringstream vals;
		for (unsigned int i=0; i<vec.size(); ++i) {
			vals << vec[i];
			if (i<vec.size()-1)  vals << ",";
		}
		TiXmlText * text = new TiXmlText( vals.str() );
		child->LinkEndChild( text );
	}
}


/*! Write vector specialization for std::string.
	Hereby, values are comma-separated without extra quotes.
*/
template <>
void writeVector(TiXmlElement * parent, const std::string & name, const std::vector<std::string> & vec);

/*! Write vector specialization for std::string.
	Hereby, values are comma-separated, quoted strings (escaped via &quot;).
*/
void writeVectorQuotedString(TiXmlElement * parent, const std::string & name, const std::vector<std::string> & vec);


template <typename T>
T readPODAttributeValue(const TiXmlElement * element, const TiXmlAttribute * attrib) {
	FUNCID(CG::readPODAttributeValue);
	try {
		return IBK::string2val<T>(attrib->ValueStr());
	} catch (IBK::Exception & ex) {
		throw IBK::Exception( ex, IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
			IBK::FormatString("Error reading '"+attrib->NameTStr()+"' attribute.") ), FUNC_ID);
	}
}

template <typename T>
T readPODElement(const TiXmlElement * element, const std::string & eName) {
	FUNCID(CG::readPODElement);
	try {
		return IBK::string2val<T>(element->GetText());
	} catch (IBK::Exception & ex) {
		throw IBK::Exception( ex, IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
			IBK::FormatString("Error reading '"+eName+"' tag.") ), FUNC_ID);
	}
}

/*! Read an IBK:Unit tag. */
IBK::Unit readUnitElement(const TiXmlElement * element, const std::string & eName);
/*! Read an IBK:Time tag. */
IBK::Time readTimeElement(const TiXmlElement * element, const std::string & eName);


template <typename T>
void readPoint2D(const TiXmlElement * element, const std::string & name, IBK::point2D<T> & p) {
	FUNCID(CG::readPoint2D);
	std::string text = element->GetText();
	try {
		typename std::vector<T> vec;
		IBK::string2vector(text, vec);
		if (vec.size() != 2)
			throw IBK::Exception("Size mismatch, expected 2 numbers separated by , .", FUNC_ID);
		p.m_x = vec[0];
		p.m_y = vec[1];
	}
	catch (IBK::Exception & ex) {
		throw IBK::Exception( ex, IBK::FormatString(XML_READ_ERROR).arg(element->Row()).arg(
			IBK::FormatString("Error reading point2D element '%1'.").arg(name) ), FUNC_ID);
	}
}

/*! Special implementation for double-type point2D. */
template <>
void readPoint2D(const TiXmlElement * element, const std::string & name, IBK::point2D<double> & p);

template <typename T>
void writePoint2D(TiXmlElement * parent, const std::string & name, const IBK::point2D<T> & p) {
	TiXmlElement * child = new TiXmlElement(name);
	parent->LinkEndChild(child);

	std::stringstream vals;
	vals << p.m_x << " " << p.m_y;
	TiXmlText * text = new TiXmlText( vals.str() );
	child->LinkEndChild( text );
}


} // namespace CODEGEN

#endif // CodeGen_UtilitiesH
