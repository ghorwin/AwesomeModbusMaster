# CMakeLists.txt file for TinyXMLCodeGenerator

project( TinyXMLCodeGenerator )

# add include directories
include_directories(
	${PROJECT_SOURCE_DIR}/../../src
	${PROJECT_SOURCE_DIR}/../../../libs/IBK/src
)

# gather all cpp files in NandradSolver directory
file( GLOB APP_SRCS ${PROJECT_SOURCE_DIR}/../../src/*.cpp )

# set variable for dependent libraries
set( LINK_LIBS
	IBK
)

# now build the NandradSolver executable - this only requires compiling the main.cpp
add_executable( ${PROJECT_NAME}
	${APP_SRCS}
)

# and link it against the dependent libraries
target_link_libraries( ${PROJECT_NAME} 
	${LINK_LIBS}
)


