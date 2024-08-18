# Qt Session file

TEMPLATE = subdirs

SUBDIRS = \
		qwt \
		libmodbus \
		AwesomeModbusMaster

# where to find the sub projects
AwesomeModbusMaster.file 	= AwesomeModbusMaster/AwesomeModbusMaster.pro
qwt.file 					= externals/qwt/qwt.pro
libmodbus.file				= externals/libmodbus/libmodbus.pro

# application and shared lib dependencies
SIM-VICUS.depends = qwt libmodbus

# library dependencies

