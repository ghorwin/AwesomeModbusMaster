#!/bin/bash


# Build script for building 'AwesomeModbusMaster' and all dependend libraries

# Command line options:
#   [reldeb|release|debug]		build type
#   [2 [1..n]]					cpu count
#   [gcc|icc]					compiler
#   [verbose]					enable cmake to call verbose makefiles
#   [skip-test]                 does not execute test-init script after successful build

# path export for mac
export PATH=~/Qt/5.11.3/gcc_64/bin:~/Qt/5.11.3/clang_64/bin:$PATH

CMAKELISTSDIR=$(pwd)/..
BUILDDIR="bb"

# set defaults
CMAKE_BUILD_TYPE=" -DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo"
MAKE_CPUCOUNT="2"
BUILD_DIR_SUFFIX="gcc"
COMPILER=""
SKIP_TESTS="false"

# parse parameters, except gprof and threadchecker
for var in "$@"
do

	if [[ $var = "--help" ]];
	then
		echo "Command line options:"
		echo "  [reldeb|release|debug]      build type"
		echo "  [2 [1..n]]                  cpu count"
		echo "  [verbose]                   enable cmake to call verbose makefiles"
		echo "  [skip-test]                 does not execute test script after successful build"

		exit
	fi

	if [[ $var = *[[:digit:]]* ]];
	then
		MAKE_CPUCOUNT=$var
		echo "Using $MAKE_CPUCOUNT CPUs for compilation"
	fi

	if [[ $var = "omp"  ]];
	then
		CMAKE_OPTIONS="$CMAKE_OPTIONS -DUSE_OMP:BOOL=ON"
		echo "Using Open MP compile flags"
	fi

	if [[ $var = "debug"  ]];
	then
		CMAKE_BUILD_TYPE=" -DCMAKE_BUILD_TYPE:STRING=Debug"
		echo "Debug build..."
	fi

	if [[ $var = "release"  ]];
	then
		CMAKE_BUILD_TYPE=" -DCMAKE_BUILD_TYPE:STRING=Release"
		echo "Release build..."
	fi

	if [[ $var = "reldeb"  ]];
	then
		CMAKE_BUILD_TYPE=" -DCMAKE_BUILD_TYPE:STRING=RelWithDebInfo"
		echo "RelWithDebInfo build..."
	fi

	if [[ $var = "verbose"  ]];
	then
		CMAKE_OPTIONS="$CMAKE_OPTIONS -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON"
	fi

	if [[ $var = "skip-test"  ]];
	then
		SKIP_TESTS="true"
	fi

done

# create build dir if not exists
BUILDDIR=$BUILDDIR-$BUILD_DIR_SUFFIX
if [ ! -d $BUILDDIR ]; then
    mkdir -p $BUILDDIR
fi

cd $BUILDDIR && cmake $CMAKE_OPTIONS $CMAKE_BUILD_TYPE $CMAKE_COMPILER_OPTIONS $CMAKELISTSDIR && make -j$MAKE_CPUCOUNT &&
cd $CMAKELISTSDIR &&
mkdir -p ../bin/release &&
if [ -e $BUILDDIR/AwesomeModbusMaster/AwesomeModbusMaster ]; then
  echo "*** Copying AwesomeModbusMaster to bin/release ***" &&
  cp $BUILDDIR/AwesomeModbusMaster/AwesomeModbusMaster ../bin/release/AwesomeModbusMaster
fi &&
# for Mac
if [ -e $BUILDDIR/AwesomeModbusMaster/AwesomeModbusMaster.app ]; then
  if [ -e ../bin/release/AwesomeModbusMaster.app ]; then
    rm -rf ../bin/release/AwesomeModbusMaster.app
  fi &&
  echo "*** Copying AwesomeModbusMaster.app to bin/release ***" &&
  cp -r $BUILDDIR/AwesomeModbusMaster/AwesomeModbusMaster.app ../bin/release/AwesomeModbusMaster.app
fi &&

echo "*** Build AwesomeModbusMaster ***" &&
if [[ $SKIP_TESTS = "false"  ]]; then
./run_tests.sh
fi

