@echo off

:: This scripts runs the code generator for library BtpCommon and generates/updates the
:: files in cg subdirectory.

:: Note: copy TinyXMLCodeGenerator into this directory first! But do not check it into repository!

copy LICENSE_HEADER ..\bin\debug\
pushd ..\bin\debug\
TinyXMLCodeGenerator.exe BTP ../../libs/BtpCommon/src+../../libs/BtpCommon/src/controlParams 0 BTP cg
popd
