/*
*/

#include <map>
#include <limits>
#include <iostream>

#include <IBK_FormatString.h>
#include <IBK_Exception.h>


#include <CodeGen_Constants.h>

#include "AMM_KeywordList.h"

namespace AMM {
	/*! Holds the string to print as error when an invalid keyword is encountered. */
	const char * const INVALID_KEYWORD_INDEX_STRING = "KEYWORD_ERROR_STRING: Invalid type index";

	/*! Holds a list of all enum types/categories. */
	const char * const ENUM_TYPES[1] = {
		"ModbusConfig::DataType"
	};

	/*! Converts a category string to respective enumeration value. */
	int enum2index(const std::string & enumtype) {
		for (int i=0; i<1; ++i) {
			if (enumtype == ENUM_TYPES[i]) return i;
		}
		//std::cerr << "Unknown enumeration type '" << enumtype<< "'." << std::endl;
		return -1;
	}
	

	/*! Returns a keyword string for a given category (typenum) and type number t. */
	const char * theKeyword(int typenum, int t) {
		switch (typenum) {
			// ModbusConfig::DataType
			case 0 :
			switch (t) {
				case 0 : return "bit";
				case 1 : return "char";
				case 2 : return "integer8";
				case 3 : return "int16_ab";
				case 4 : return "int16_ba";
				case 5 : return "long32_cdab";
				case 6 : return "long32_abcd";
				case 7 : return "long32_dcba";
				case 8 : return "long32_badc";
				case 9 : return "integer64_abcd";
				case 10 : return "float32_cdab";
				case 11 : return "float32_abcd";
				case 12 : return "float32_dcba";
				case 13 : return "float32_badc";
			} break;
		} // switch
		return INVALID_KEYWORD_INDEX_STRING;
	}

	/*! Returns all keywords including deprecated for a given category (typenum) and type number (t). */
	const char * allKeywords(int typenum, int t) {
		switch (typenum) {
			// ModbusConfig::DataType
			case 0 :
			switch (t) {
				case 0 : return "bit";
				case 1 : return "char";
				case 2 : return "integer8";
				case 3 : return "int16_ab";
				case 4 : return "int16_ba";
				case 5 : return "long32_cdab";
				case 6 : return "long32_abcd";
				case 7 : return "long32_dcba";
				case 8 : return "long32_badc";
				case 9 : return "integer64_abcd";
				case 10 : return "float32_cdab";
				case 11 : return "float32_abcd";
				case 12 : return "float32_dcba";
				case 13 : return "float32_badc";
			} break;
		} // switch
		return INVALID_KEYWORD_INDEX_STRING;
	}

	const char * KeywordList::Description(const char * const enumtype, int t, bool * no_description) {
		if (no_description != nullptr)
			*no_description = false; // we are optimistic
		switch (enum2index(enumtype)) {
			// ModbusConfig::DataType
			case 0 :
			switch (t) {
				case 0 : if (no_description != nullptr) *no_description = true; return "bit";
				case 1 : if (no_description != nullptr) *no_description = true; return "char";
				case 2 : if (no_description != nullptr) *no_description = true; return "integer8";
				case 3 : if (no_description != nullptr) *no_description = true; return "int16_ab";
				case 4 : if (no_description != nullptr) *no_description = true; return "int16_ba";
				case 5 : if (no_description != nullptr) *no_description = true; return "long32_cdab";
				case 6 : if (no_description != nullptr) *no_description = true; return "long32_abcd";
				case 7 : if (no_description != nullptr) *no_description = true; return "long32_dcba";
				case 8 : if (no_description != nullptr) *no_description = true; return "long32_badc";
				case 9 : if (no_description != nullptr) *no_description = true; return "integer64_abcd";
				case 10 : if (no_description != nullptr) *no_description = true; return "float32_cdab";
				case 11 : if (no_description != nullptr) *no_description = true; return "float32_abcd";
				case 12 : if (no_description != nullptr) *no_description = true; return "float32_dcba";
				case 13 : if (no_description != nullptr) *no_description = true; return "float32_badc";
			} break;
		} // switch
		throw IBK::Exception(IBK::FormatString("Cannot determine description for enumeration type '%1' and index '%2'.")
			.arg(enumtype).arg(t), "[KeywordList::Description]");
	}

	const char * KeywordList::Unit(const char * const enumtype, int t) {
		switch (enum2index(enumtype)) {
			// ModbusConfig::DataType
			case 0 :
			switch (t) {
				case 0 : return "";
				case 1 : return "";
				case 2 : return "";
				case 3 : return "";
				case 4 : return "";
				case 5 : return "";
				case 6 : return "";
				case 7 : return "";
				case 8 : return "";
				case 9 : return "";
				case 10 : return "";
				case 11 : return "";
				case 12 : return "";
				case 13 : return "";
			} break;
		} // switch
		throw IBK::Exception(IBK::FormatString("Cannot determine default unit for enumeration type '%1' and index '%2'.")
			.arg(enumtype).arg(t), "[KeywordList::Unit]");
	}

	const char * KeywordList::Color(const char * const enumtype, int t) {
		switch (enum2index(enumtype)) {
			// ModbusConfig::DataType
			case 0 :
			switch (t) {
				case 0 : return "#FFFFFF";
				case 1 : return "#FFFFFF";
				case 2 : return "#FFFFFF";
				case 3 : return "#FFFFFF";
				case 4 : return "#FFFFFF";
				case 5 : return "#FFFFFF";
				case 6 : return "#FFFFFF";
				case 7 : return "#FFFFFF";
				case 8 : return "#FFFFFF";
				case 9 : return "#FFFFFF";
				case 10 : return "#FFFFFF";
				case 11 : return "#FFFFFF";
				case 12 : return "#FFFFFF";
				case 13 : return "#FFFFFF";
			} break;
		} // switch
		throw IBK::Exception(IBK::FormatString("Cannot determine color for enumeration type '%1' and index '%2'.")
			.arg(enumtype).arg(t), "[KeywordList::Color]");
	}

	double KeywordList::DefaultValue(const char * const enumtype, int t) {
		switch (enum2index(enumtype)) {
			// ModbusConfig::DataType
			case 0 :
			switch (t) {
				case 0 : return std::numeric_limits<double>::quiet_NaN();
				case 1 : return std::numeric_limits<double>::quiet_NaN();
				case 2 : return std::numeric_limits<double>::quiet_NaN();
				case 3 : return std::numeric_limits<double>::quiet_NaN();
				case 4 : return std::numeric_limits<double>::quiet_NaN();
				case 5 : return std::numeric_limits<double>::quiet_NaN();
				case 6 : return std::numeric_limits<double>::quiet_NaN();
				case 7 : return std::numeric_limits<double>::quiet_NaN();
				case 8 : return std::numeric_limits<double>::quiet_NaN();
				case 9 : return std::numeric_limits<double>::quiet_NaN();
				case 10 : return std::numeric_limits<double>::quiet_NaN();
				case 11 : return std::numeric_limits<double>::quiet_NaN();
				case 12 : return std::numeric_limits<double>::quiet_NaN();
				case 13 : return std::numeric_limits<double>::quiet_NaN();
			} break;
		} // switch
		throw IBK::Exception(IBK::FormatString("Cannot determine default value for enumeration type '%1' and index '%2'.")
			.arg(enumtype).arg(t), "[KeywordList::DefaultValue]");
	}

	// number of entries in a keyword list
	unsigned int KeywordList::Count(const char * const enumtype) {
		switch (enum2index(enumtype)) {
			// ModbusConfig::DataType
			case 0 : return 14;
		} // switch
		throw IBK::Exception(IBK::FormatString("Invalid enumeration type '%1'.")
			.arg(enumtype), "[KeywordList::Count]");
	}

	// max index for entries sharing a category in a keyword list
	int KeywordList::MaxIndex(const char * const enumtype) {
		switch (enum2index(enumtype)) {
			// ModbusConfig::DataType
			case 0 : return 14;
		} // switch
		throw IBK::Exception(IBK::FormatString("Invalid enumeration type '%1'.")
			.arg(enumtype), "[KeywordList::MaxIndex]");
	}

	const char * KeywordList::Keyword(const char * const enumtype, int t) {
		const char * const kw = theKeyword(enum2index(enumtype), t);
		if (std::string(kw) == INVALID_KEYWORD_INDEX_STRING) {
			throw IBK::Exception(IBK::FormatString("Cannot determine keyword for enumeration type '%1' and index '%2'.")
				.arg(enumtype).arg(t), "[KeywordList::Keyword]");
		}
		return kw;
	}

	bool KeywordList::KeywordExists(const char * const enumtype, const std::string & kw) {
		int typenum = enum2index(enumtype);
		int i = 0;
		int maxIndexInCategory = MaxIndex( enumtype ); 
		for ( ; i <= maxIndexInCategory; ++i ) {
			std::string keywords = allKeywords(typenum, i);
			if (keywords == INVALID_KEYWORD_INDEX_STRING)
				continue;
			std::stringstream strm(keywords);
			int j = 0;
			std::string kws;
			while (strm >> kws) {
				if (kws == kw) {
					return true; // keyword exists
				}
				++j;
			}
		}
		return false; // nothing found keyword doesn't exist.
	}

	int KeywordList::Enumeration(const char * const enumtype, const std::string & kw, bool * deprecated) {
		int typenum = enum2index(enumtype);
		int i = 0;
		int maxIndexInCategory = MaxIndex( enumtype ); 
		for ( ; i <= maxIndexInCategory; ++i ) {
			std::string keywords = allKeywords(typenum, i);
			if (keywords == INVALID_KEYWORD_INDEX_STRING)
				continue;
			std::stringstream strm(keywords);
			int j = 0;
			std::string kws;
			while (strm >> kws) {
				if (kws == kw) {
					if (deprecated != nullptr) {
						*deprecated = (j != 0);
					}
					return i;
				}
				++j;
			}
		} // for ( ; i < maxIndexInCategory; ++i ) {
		throw IBK::Exception(IBK::FormatString("Cannot determine enumeration value for "
			"enumeration type '%1' and keyword '%2'.")
			.arg(enumtype).arg(kw), "[KeywordList::Enumeration]");
	}

	bool KeywordList::CategoryExists(const char * const enumtype) {
		return enum2index(enumtype) != -1;
	}

	void KeywordList::setParameter(IBK::Parameter para[], const char * const enumtype, int n, const double &val) {
		para[n] = IBK::Parameter(Keyword(enumtype, n), val, Unit(enumtype, n));
	}

	void KeywordList::setIntPara(IBK::IntPara para[], const char * const enumtype, int n, const int &val) {
		para[n] = IBK::IntPara(Keyword(enumtype, n), val);
	}

} // namespace AMM
