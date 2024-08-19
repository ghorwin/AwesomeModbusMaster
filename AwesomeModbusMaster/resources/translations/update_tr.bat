@echo off
set QTDIR=C:\Qt\5.15.2\msvc2019_64
set PATH=c:\Qt\5.15.2\msvc2019_64\bin;%PATH%

:: setup VC environment variables
set VCVARSALL_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
call %VCVARSALL_PATH%

rem 1. Call this batch file to update the ts files.
rem 2. Then edit the ts files with Qt Linguist
rem 3. Then call release_tr.bat to generate release files
rem lupdate ../../../libs/QtExt/projects/Qt/QtExt.pro
lupdate ../../AwesomeModbusMaster.pro

echo Translation files (*.ts) updated, use Qt Linguist to add missing translations!
PAUSE

linguist AwesomeModbusMaster_de.ts
rem linguist AwesomeModbusMaster_de.ts ../../../libs/QtExt/resources/translations/QtExt_de.ts

