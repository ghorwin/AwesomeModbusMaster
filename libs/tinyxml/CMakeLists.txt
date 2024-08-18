# CMakeLists.txt file for TiCPP library

project( TiCPP )

# collect a list of all source files of the library
file (GLOB TiCPP_LIB_SRCS src/*.cpp )

# build the library
add_library( ${PROJECT_NAME} STATIC
	${TiCPP_LIB_SRCS}
)

