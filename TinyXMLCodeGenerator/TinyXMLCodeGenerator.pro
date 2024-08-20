# Project file for TinyXMLCodeGenerator

TARGET = TinyXMLCodeGenerator
TEMPLATE = app

CONFIG += console

# this pri must be sourced from all our libraries,
# it contains all functions defined for casual libraries
include( ../libs/common.pri )

DEPENDPATH = $${INCLUDEPATH}

INCLUDEPATH = ../libs/IBK/src

LIBS += -lIBK

SOURCES += \
	src/ClassInfo.cpp \
	src/CodeGenerator.cpp \
	src/constants.cpp \
	src/main.cpp

HEADERS += \
	src/ClassInfo.h \
	src/CodeGenerator.h \
	src/constants.h
