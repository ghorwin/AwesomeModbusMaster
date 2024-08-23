#include "Exception.h"
#include <QDebug>

#include "MessageHandler.h"

Exception::Exception(const QString& what, const QString& loc) {
	m_msgs.push_back( Exception::MsgInfo(what, loc) );
}


Exception::Exception(const Exception & old, const QString& what, const QString& loc) :
	m_msgs(old.m_msgs)
{
	m_msgs.push_back( Exception::MsgInfo(what, loc) );
}


Exception::Exception(const std::list<MsgInfo> & msgs, const QString& what, const QString& loc) :
	m_msgs(msgs)
{
	m_msgs.push_back( Exception::MsgInfo(what, loc) );
}


const char* Exception::what() const throw() {
	static std::string msg;
	if (m_msgs.empty()) return "";
	msg = m_msgs.back().what.toStdString();
	return msg.c_str();
}


QString Exception::location() const {
	if (m_msgs.empty()) return "";
	else return m_msgs.back().location;
}


void Exception::writeMsgStackToError() const {
	for (std::list<MsgInfo>::const_iterator it = m_msgs.begin();
		it != m_msgs.end(); ++it)
	{
		MessageHandler::instance().msg(QtCriticalMsg, QMessageLogContext(it->location.toStdString().c_str(), 0, "","exception"), it->what);
	}
}


