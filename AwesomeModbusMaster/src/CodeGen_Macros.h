// CodeGenMacros - based on NANDRAD_CodeGenMacros.h from SIM-VICUS project

#ifndef CODEGEN_CodeGenMacrosH
#define CODEGEN_CodeGenMacrosH

class TiXmlElement;

// IDType is used instead of unsigned int for special serialization feature
typedef unsigned int IDType;
// QuotedString is used to distinguish serialization with and without extra quotes
#define QuotedString std::string

#define CODEGEN_READWRITE \
	void readXML(const TiXmlElement * element); \
	TiXmlElement * writeXML(TiXmlElement * parent) const;

#define CODEGEN_READWRITE_IFNOTEMPTY(X) \
	void readXML(const TiXmlElement * element) { readXMLPrivate(element); } \
	TiXmlElement * writeXML(TiXmlElement * parent) const { if (*this != X()) return writeXMLPrivate(parent); else return nullptr; }

#define CODEGEN_READWRITE_IFNOT_INVALID_ID \
	void readXML(const TiXmlElement * element) { readXMLPrivate(element); } \
	TiXmlElement * writeXML(TiXmlElement * parent) const { if (m_id != INVALID_ID) return writeXMLPrivate(parent); else return nullptr; }

#define CODEGEN_READWRITE_PRIVATE \
	void readXMLPrivate(const TiXmlElement * element); \
	TiXmlElement * writeXMLPrivate(TiXmlElement * parent) const;

#define CODEGEN_COMP(X) \
	bool operator!=(const X & other) const; \
	bool operator==(const X & other) const { return !operator!=(other); }

#define CODEGEN_COMPARE_WITH_ID \
	bool operator==(unsigned int x) const { return m_id == x; }

#define CODEGEN_LESS_ID(X) \
	bool operator<(const X & x) const { return m_id < x.m_id; }

#define CODEGEN_COMPARE_WITH_NAME \
	bool operator==(const std::string & name) const { return m_name == name; }

#endif // CODEGEN_CodeGenMacrosH
