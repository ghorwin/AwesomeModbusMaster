# CMakeLists.txt file

project( libmodbus C )

# collect a list of all source files of the library
file (GLOB LIB_SRCS ${PROJECT_SOURCE_DIR}/../../src/*.c )

include_directories(
	${PROJECT_SOURCE_DIR}/../../src
	${PROJECT_SOURCE_DIR}/../..
)

# build the library
add_library( ${PROJECT_NAME} STATIC
	${LIB_SRCS}
)
