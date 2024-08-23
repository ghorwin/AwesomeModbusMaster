# CMakeLists.txt file for AwesomeModbusMaster

project( AwesomeModbusMaster )

if (COMMAND cmake_policy)
  # set linker search policy new
  cmake_policy(SET CMP0003 NEW)
  # Policy CMP0020: Automatically link Qt executables to qtmain target on Windows.
  cmake_policy(SET CMP0020 NEW)
endif (COMMAND cmake_policy)

# add include directories
include_directories(
	${PROJECT_SOURCE_DIR}/../../src			# needed so that ui-generated header files find our own headers
	${PROJECT_SOURCE_DIR}/../../../libs/IBK/src
	${PROJECT_SOURCE_DIR}/../../../libs/TiCPP/src
	${PROJECT_SOURCE_DIR}/../../../libs/qwt/src
	${PROJECT_SOURCE_DIR}/../../../libs/libmodbus/src
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Svg_INCLUDE_DIRS}
	${Qt5PrintSupport_INCLUDE_DIRS}
)

# collect a list of all source files
file( GLOB APP_SRCS 		${PROJECT_SOURCE_DIR}/../../src/*.cpp )

# collect a list of all header files (to be used in MOC compiler)
file( GLOB APP_HDRS 		${PROJECT_SOURCE_DIR}/../../src/*.h )

# collect a list of all form files
file( GLOB APP_UIS 			${PROJECT_SOURCE_DIR}/../../src/*.ui )

# add Qt-Keywordlist files
set( APP_SRCS
	${APP_SRCS}
#	${PROJECT_SOURCE_DIR}/../../NANDRAD_KeywordListQt.cpp
)

# The Qt resource file (*.qrc)
set( APP_QRCS	
	${PROJECT_SOURCE_DIR}/../../resources/AwesomeModbusMaster.qrc 
)

# set rc file
set ( APP_WIN_RC
	${PROJECT_SOURCE_DIR}/../../resources/AwesomeModbusMaster.rc
)

# run uic and moc compilers, add resources

qt5_add_resources( APP_RC_SRCS ${APP_QRCS} )
qt5_wrap_ui( APP_UI_SRCS ${APP_UIS} )
qt5_wrap_cpp( APP_MOC_SRCS ${APP_HDRS} )

# generate list of all required QT_LIBRARIES
set ( QT_LIBRARIES 
	Qt5::Widgets
	Qt5::PrintSupport
	Qt5::Svg
	Qt5::Concurrent
)

# build application executable for the different platforms
if( WIN32 )
	add_executable( ${PROJECT_NAME} WIN32 
					 ${APP_SRCS} ${APP_MOC_SRCS} ${APP_RC_SRCS} ${APP_UI_SRCS} ${APP_WIN_RC}
	)

	set(WIN_FRAMEWORKS iphlpapi gdiplus gdi32 user32 ole32)
	
	# enable console window (to see debug/profiler messages)
	# Mind: when commenting out next line, you need to link against WinMain
	#set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE" )
	
endif( WIN32 ) 

# Note: this is just so that the manually build app bundle is basically configured when testing in 
#       MACOS and has a decent icon. In deployment mode, the same PList file needs to be used.
if( APPLE )
  set( GUI_TYPE MACOSX_BUNDLE )
  SET_SOURCE_FILES_PROPERTIES( ${PROJECT_SOURCE_DIR}/../../resources/gfx/AwesomeModbusMaster.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif( APPLE )

if( UNIX )
	if( APPLE )
		add_executable( ${PROJECT_NAME} MACOSX_BUNDLE 
						${APP_SRCS} ${APP_MOC_SRCS} ${APP_RC_SRCS} ${APP_UI_SRCS}
						${PROJECT_SOURCE_DIR}/../../resources/gfx/AwesomeModbusMaster.icns
		)
	else( APPLE )
		add_executable( ${PROJECT_NAME} 
						${APP_SRCS} ${APP_MOC_SRCS} ${APP_RC_SRCS} ${APP_UI_SRCS} 
						
		)
	endif( APPLE ) 

endif( UNIX )


# link against the dependent libraries
target_link_libraries( ${PROJECT_NAME} 
	libmodbus
	TiCPP
	IBK
	qwt6
	# NOTE:  DO NOT LINK against Licensing
	${QT_LIBRARIES}
	${APPLE_FRAMEWORKS}
	${WIN_FRAMEWORKS}
)



