# Project file for AwesomeModbusMaster
#
# remember to set DYLD_FALLBACK_LIBRARY_PATH on MacOSX
# set LD_LIBRARY_PATH on Linux

TARGET = AwesomeModbusMaster
TEMPLATE = app

# this pri must be sourced from all our libraries,
# it contains all functions defined for casual libraries
include( ../libs/common.pri )

# console only for debugging purposes
CONFIG(debug, debug|release) {
	CONFIG += console
#	DEFINES += QT_DEBUG # not set when building with MinGW
}

CONFIG(release, debug|release) {
	# comment this out if you are NOT deploying the application but merely testing in release mode
	DEFINES += DEPLOYMENT
	# comment this in, if you want assertions also in release code
	DEFINES += QT_FORCE_ASSERTS
}

QT += core gui widgets network

INCLUDEPATH = \
	../libs/tinyxml/src \
	../libs/libmodbus/src \
	../libs/qwt/src 
	src

LIBS += \
	-lqwt \
	-ltinyxml \
	-llibmodbus


win32 {
	LIBS += -lws2_32
}

SOURCES += \
	src/MainWindow.cpp \
	src/main.cpp

HEADERS +=  \
	src/MainWindow.h


RESOURCES += \
	resources/AwesomeModbusMaster.qrc

TRANSLATIONS += resources/translations/AwesomeModbusMaster_de.ts

CODECFORSRC = UTF-8

FORMS += \
	src/MainWindow.ui
