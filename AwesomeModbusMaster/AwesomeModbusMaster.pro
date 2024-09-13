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
	../libs/qwt/src \
	../libs/IBK/src \
	src

LIBS += \
	-lIBK \
	-lqwt \
	-ltinyxml \
	-llibmodbus


win32 {
	LIBS += -lws2_32
}

SOURCES += \
	src/AMM_Config.cpp \
	src/AMM_ModbusConfig.cpp \
	src/ClickableLabel.cpp \
	src/CodeGen_Constants.cpp \
	src/CodeGen_Utilities.cpp \
	src/Constants.cpp \
	src/DebugApplication.cpp \
	src/Directories.cpp \
	src/Exception.cpp \
	src/LanguageHandler.cpp \
	src/LogFileDialog.cpp \
	src/LogWidget.cpp \
	src/MainWindow.cpp \
	src/MessageHandler.cpp \
	src/ProjectHandler.cpp \
	src/Settings.cpp \
	src/StringUtils.cpp \
	src/Style.cpp \
	src/UndoCommandBase.cpp \
	src/UndoModifyProject.cpp \
	src/WelcomeScreen.cpp \
	src/cg/cg_AMM_KeywordList.cpp \
	src/cg/cg_AMM_ModbusConfig.cpp \
	src/cg/cg_AMM_ModbusConnection.cpp \
	src/main.cpp

HEADERS += \
	src/AMM_Config.h \
	src/AMM_KeywordList.h \
	src/AMM_ModbusConfig.h \
	src/AMM_ModbusConnection.h \
	src/ClickableLabel.h \
	src/CodeGen_Constants.h \
	src/CodeGen_Macros.h \
	src/CodeGen_Utilities.h \
	src/Constants.h \
	src/DebugApplication.h \
	src/Directories.h \
	src/Exception.h \
	src/LanguageHandler.h \
	src/LogFileDialog.h \
	src/LogWidget.h \
	src/MainWindow.h \
	src/MessageHandler.h \
	src/ProjectHandler.h \
	src/Settings.h \
	src/StringUtils.h \
	src/Style.h \
	src/UndoCommandBase.h \
	src/UndoModifyProject.h \
	src/WelcomeScreen.h

FORMS += \
	src/LogFileDialog.ui \
	src/MainWindow.ui \
	src/WelcomeScreen.ui

RESOURCES += \
	resources/AwesomeModbusMaster.qrc

TRANSLATIONS += resources/translations/AwesomeModbusMaster_de.ts

CODECFORSRC = UTF-8

