# Project file for AwesomeModbusMaster
#
# remember to set DYLD_FALLBACK_LIBRARY_PATH on MacOSX
# set LD_LIBRARY_PATH on Linux

TARGET = AwesomeModbusMaster
TEMPLATE = app

# this pri must be sourced from all our libraries,
# it contains all functions defined for casual libraries
include( ../libs/UitCore/UIT.pri )

# console only for debugging purposes
CONFIG(debug, debug|release) {
	CONFIG += console
#	DEFINES += QT_DEBUG # not set when building with MinGW
	#DEFINES += USE_LOCAL_DB # to use local DB connection for development on linux
}

CONFIG(release, debug|release) {
	# comment this in if you are deploying the application
	DEFINES += DEPLOYMENT
	# comment this in, if you want assertions also in release code
	DEFINES += QT_FORCE_ASSERTS
}

QT += core gui sql widgets network

INCLUDEPATH = \
	../libs/UitCore/src \
	../libs/libmodbus/src \
	../libs/qwt/src 
	src

LIBS += \
	-lUitCore \
	-ltinyxml \
	-llibmodbus


win32 {
	LIBS += -lws2_32
}

SOURCES += \
	../BtpReactorGUI/src/BtpStyle.cpp \
	../BtpReactorGUI/src/Settings.cpp \
	src/Constants.cpp \
	src/MainWindow.cpp \
	src/ModbusTcpServer.cpp \
	src/AwesomeModbusMaster.cpp \
	src/main.cpp \
	src/ttcpserver.cpp

HEADERS += \
	../BtpReactorGUI/src/BtpStyle.h \
	../BtpReactorGUI/src/Settings.h \
	src/Constants.h \
	src/MainWindow.h \
	src/ModbusTcpServer.h \
	src/AwesomeModbusMaster.h \
	src/ttcpserver.h

FORMS += \
	src/MainWindow.ui

RESOURCES += \
	../BtpReactorGUI/resources/BtpReactorGUI.qrc
