#!/bin/bash

# This scripts runs the code generator for library BtpCommon and generates/updates the
# files in cg subdirectory.

export LD_LIBRARY_PATH=$(pwd)/../../build-AwesomeModbusMaster-session-Desktop-Debug/libs/lib/debug/

(cd ../bin/debug;./TinyXMLCodeGenerator AMM ../../AwesomeModbusMaster/src 0 AMM cg)

