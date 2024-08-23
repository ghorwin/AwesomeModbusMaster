#include "StringUtils.h"

#include <QString>
#include <QList>
#include <QFile>
#include <QDebug>
#include <QtCore/QLoggingCategory>

#include <iostream>

#ifdef _WIN32
#ifdef NOMINMAX
#include <windows.h>
#else
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#endif
#endif

Q_LOGGING_CATEGORY(lcCoreStringUtils, "core.stringutils")

#ifdef _WIN32

std::wstring UTF8ToWstring(const std::string& utf8str) {
	if (utf8str.empty())
		return std::wstring();

	int reslength = MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, 0, 0);
	if (reslength == 0)
		return std::wstring();

	std::vector<wchar_t> wide(reslength, L'\0');
	int writtenLength = MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, &wide[0], reslength);
	if (writtenLength == 0)
		return std::wstring();

	return std::wstring(&wide[0]);
}


std::string WstringToUTF8(const std::wstring& wide) {
	if (wide.empty())
		return std::string();

	int reslength = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, 0, 0, 0, 0);
	if (reslength == 0)
		return std::string();

	std::vector<char> str(reslength, '\0');
	int writtenLength = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, &str[0], reslength, 0, 0);
	if (writtenLength == 0)
		return std::string();

	return std::string(&str[0]);
}


std::wstring ANSIToWstring(const std::string& ansiString, bool OEMPage) {
	if (ansiString.empty())
		return std::wstring();

	DWORD pageFlag = OEMPage ? CP_OEMCP : CP_ACP;
	int reslength = MultiByteToWideChar(pageFlag, 0, ansiString.c_str(), -1, 0, 0);
	if (reslength == 0)
		return std::wstring();

	std::vector<wchar_t> wide(reslength, L'\0');
	int writtenLength = MultiByteToWideChar(pageFlag, 0, ansiString.c_str(), -1, &wide[0], reslength);
	if (writtenLength == 0)
		return std::wstring();

	return std::wstring(&wide[0]);
}


std::string WstringToANSI(const std::wstring& wide, bool OEMPage) {
	if(wide.empty())
		return std::string();

	DWORD pageFlag = OEMPage ? CP_OEMCP : CP_ACP;
	int reslength = WideCharToMultiByte(pageFlag, 0, wide.c_str(), -1, 0, 0, 0, 0);
	if(reslength == 0)
		return std::string();

	std::vector<char> str(reslength, '\0');
	int writtenLength = WideCharToMultiByte(pageFlag, 0, wide.c_str(), -1, &str[0], reslength, 0, 0);
	if(writtenLength == 0)
		return std::string();

	return std::string(&str[0]);
}

#endif // _WIN32

/*! Converts a string literal of format 'Andreas N <a.nicolai@dort.hier>; Ben X <b.x@dort.hier>'
into 2 stringlists with name and email addresses separated. */
void emailReceivers2StringLists(const char * const emailReceivers, QStringList & names, QStringList & emailadds) {
	QStringList emails = QString(emailReceivers).split(";");
	for (const QString & e : qAsConst(emails)) {
		int pos = e.indexOf('<');
		int pos2 = e.indexOf('>');
		if (pos == -1 || pos2 == -1 || pos > pos2)
			continue; // malformed
		QString name = e.left(pos).trimmed();
		QString add = e.mid(pos+1, pos2-pos-1).trimmed();
		names.append(name);
		emailadds.append(add);
	}
}

void emailReceivers2StringLists(const QString & emailReceivers, QStringList & names, QStringList & emailadds)
{
	emailReceivers2StringLists(emailReceivers.toStdString().c_str(),names,emailadds);
}

QList<unsigned int> split2UIntList(const QString & text, const QString& sep) {
	QStringList sl = text.split(sep);
	QList<unsigned int> uintList;
	foreach (QString s, sl) {
		if (s.isEmpty())
			continue;
		bool ok;
		unsigned int i = s.toUInt(&ok);
		if (ok)
			uintList.append(i);
	}
	return uintList;
}

QString readRessourceFile(QString filename) {
	QFile f(filename);
	if (!f.open(QFile::ReadOnly)) {
		qCCritical(lcCoreStringUtils) << "Cannot open file" << filename;
	}
	return f.readAll();
}

