# Project file for tinyxml

TARGET = tinyxml
TEMPLATE = lib

# this pri must be sourced from all our libraries,
# it contains all functions defined for casual libraries
include( ../common.pri )

# we want the tinyxml lib always as static library
CONFIG += static

CONFIG += console
CONFIG -= qt

LIBS += -lIBK

INCLUDEPATH = \
	../IBK/src

HEADERS += \
	src/tinystr.h \
	src/tinyxml.h

SOURCES += \
	src/tinyxmlparser.cpp \
	src/tinystr.cpp \
	src/tinyxml.cpp \
	src/tinyxmlerror.cpp


