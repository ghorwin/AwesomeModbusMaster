@echo off
set QTDIR=C:\Qt\5.15.2\msvc2019_64
set PATH=c:\Qt\5.15.2\msvc2019_64\bin;%PATH%

:: setup VC environment variables
set VCVARSALL_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"
call %VCVARSALL_PATH%

lrelease AwesomeModbusMaster_de.ts -qm AwesomeModbusMaster_de.qm
:: lrelease ../../../externals/QtExt/resources/translations/QtExt_es.ts AwesomeModbusMaster_es.ts -qm AwesomeModbusMaster_es.qm

pause
