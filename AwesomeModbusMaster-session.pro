# Qt Session file

TEMPLATE = subdirs

SUBDIRS = \
		qwt \
		IBK \
		tinyxml \
		libmodbus \
		AwesomeModbusMaster \
		TinyXMLCodeGenerator

# where to find the sub projects
qwt.file 					= libs/qwt/qwt.pro
IBK.file 					= libs/IBK/IBK.pro
tinyxml.file				= libs/tinyxml/tinyxml.pro
libmodbus.file				= libs/libmodbus/libmodbus.pro

#AwesomeModbusMaster.file	= AwesomeModbusMaster/AwesomeModbusMaster.pro

# application and shared lib dependencies
AwesomeModbusMaster.depends = qwt libmodbus tinyxml IBK
TinyXMLCodeGenerator.depends = IBK

# library dependencies

