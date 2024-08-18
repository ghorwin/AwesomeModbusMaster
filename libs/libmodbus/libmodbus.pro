# Project file for libmodbus
# Library is expected to be placed in same directory as
# UitCore

TARGET = libmodbus
TEMPLATE = lib

# this pri must be sourced from all our libraries,
# it contains all functions defined for casual libraries
include( ../UitCore/UIT.pri )

# we want the lib as shared library on Windows
win32 {
	CONFIG += shared
	# set the define on Windows to generate DLL_EXPORT statements
	DEFINES += DLLBUILD
}


CONFIG += console

# enable function/file info also in release mode
DEFINES += QT_MESSAGELOGCONTEXT

win32 {
	# Mind: when building x64 code, place the x64 version of ws2_32.dll (same name!) into exe directory.
	LIBS += -lws2_32
}

INCLUDEPATH += src   # for config.h

HEADERS += \
	src/config.h \
	src/modbus-private.h \
	src/modbus-rtu-private.h \
	src/modbus-rtu.h \
	src/modbus-tcp-private.h \
	src/modbus-tcp.h \
	src/modbus-version.h \
	src/modbus-version.h.in \
	src/modbus.h

SOURCES += \
	src/modbus-data.c \
	src/modbus-rtu.c \
	src/modbus-tcp.c \
	src/modbus.c
