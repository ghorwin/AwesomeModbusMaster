#ifndef StringUtilsH
#define StringUtilsH

#include <QString>
#include <QList>

// windows specific conversion functions
#if defined(_WIN32)

/*! Converts a given UTF8 encoded string into a unicode string.*/
std::wstring UTF8ToWstring(const std::string& utf8str);

/*! Converts a given unicode string into a UTF8 encoded string.*/
std::string WstringToUTF8(const std::wstring& wide);

/*! Converts the given ANSI string to a wide string (UTF16).
	The given ANSI string must be encoded with the standard codepage or the OEM codepage.*/
std::wstring ANSIToWstring(const std::string& ansiString, bool OEMPage);

/*! Converts the given wide string (UTF16) to a ANSI string.
	The given ANSI string will be encoded with the standard codepage or the OEM codepage.*/
std::string WstringToANSI(const std::wstring& wide, bool OEMPage);

/*! Converts a given UTF8 string into a codepage string.*/
inline std::string UTF8ToANSIString(const std::string& utf8str) {
	return  WstringToANSI(UTF8ToWstring(utf8str), false);
}

inline std::string ANSIToUTF8String(const std::string& ansiString) {
	return WstringToUTF8(ANSIToWstring(ansiString, false));
}

#endif

/*! Converts a string literal of format 'Andreas N <a.nicolai@dort.hier>; Ben X <b.x@dort.hier>'
into 2 stringlists with name and email addresses separated. */
void emailReceivers2StringLists(const char * const emailReceivers, QStringList & names, QStringList & emailadds);

/*! Converts a string literal of format 'Andreas N <a.nicolai@dort.hier>; Ben X <b.x@dort.hier>'
into 2 stringlists with name and email addresses separated.
Overload for QString.
*/
void emailReceivers2StringLists(const QString & emailReceivers, QStringList & names, QStringList & emailadds);

/*! Convenience function, wraps text in quotes.
	Use for CSV export.
*/
inline QString quote(const QString & text) {
	return "\"" + text + "\"";
}

/*! Converts a list of any value type into a string.
	\code
	// convert a QList<int> into values
	QList<int> vals;
	QString cols = Uit::join(m_clientTableColWidths, " ", [](const int &item)->QString{ return QString::number(item); });

	QList<double>  vals;
	QString cols = Uit::join(m_clientTableColWidths, " ", [precision](const double &item)->QString{ return QString::number(item, 'f', precision); });
	\endcode
*/
template <class T>
static QString join(const QList<T> &lst,
					const QString &sep,
					const std::function< QString (const typename QList<T>::value_type &) > toStringFunction)
{
	QString out;
	for (int i = 0; i<lst.size(); i++) {
		out += toStringFunction(lst[i]);
		if (i+1<lst.size())
			out += sep;
	}
	return out;
}

/*! Converts a string of format "12 12 12" (or using any other separator) into a list of unsigned ints.
	Invalid numbers/texts are silently ignored.
*/
QList<unsigned int> split2UIntList(const QString & text, const QString& sep);


/*! Convenience function to read a text from a Qt ressource.
	\param filename For example ':/data/text.txt'
*/
QString readRessourceFile(QString filename);


#endif // StringUtilsH
