# Qt Session file

TEMPLATE = subdirs

SUBDIRS = \
		qwt \
		tinyxml \
		libmodbus \
		AwesomeModbusMaster

# where to find the sub projects
qwt.file 					= libs/qwt/qwt.pro
tinyxml.file				= libs/tinyxml/tinyxml.pro
libmodbus.file				= libs/libmodbus/libmodbus.pro

AwesomeModbusMaster.file	= AwesomeModbusMaster/AwesomeModbusMaster.pro

# application and shared lib dependencies
AwesomeModbusMaster.depends = qwt libmodbus tinyxml

# library dependencies

