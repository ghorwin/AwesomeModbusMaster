# CMakeLists.txt file for IBK library

project( IBK )

# configuration file options
option( IBK_ENABLE_SAFE_MATH "Turns checking of function arguments to math routines on." ON )
option( IBK_ENABLE_UNIT_NAME "Adds a member variable with unit ID name to the IBK::Unit for better debugging." ON )
option( IBK_ENABLE_COLORED_CONSOLE "Enables colored outputs in the console window." ON )

# The top-level CMakeLists.txt file for debian package building sets this option
option( IBK_BUILDING_DEBIAN_PACKAGE "Set to true if a debian package is being built." OFF )

# avoid warning about unknown pragmas
if( UNIX )
	add_definitions( -Wunknown-pragmas )
endif( UNIX )

configure_file(
	${PROJECT_SOURCE_DIR}/../../src/IBK_configuration.h.in
	${PROJECT_SOURCE_DIR}/../../src/IBK_configuration.h
)

# collect a list of all source files of the IBK library
file( GLOB LIB_SRCS ${PROJECT_SOURCE_DIR}/../../src/*.cpp )

# build the library
add_library( ${PROJECT_NAME} STATIC
	${LIB_SRCS}
)

