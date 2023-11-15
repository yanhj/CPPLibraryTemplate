@echo off
setlocal enabledelayedexpansion

echo "=============================="
set CURRENT_DIR=%~dp0
call %CURRENT_DIR%..\depends\config.bat
call %CURRENT_DIR%..\depends\default.config.bat


echo "BUILD_ROOT=%BUILD_ROOT%"
if exist "%BUILD_ROOT%" (
	echo rmdir /Q /S %BUILD_ROOT%
	rmdir /Q /S %BUILD_ROOT%
)

REM Qt

set QT_PATH=%QT_5_15_13_HOME%


REM 解決斜杠在路徑解析異常問題 replace \\ to /
set QT_PATH=%QT_PATH:/=\%
set QT_PATH=%QT_PATH:\=/%

echo "QT_PATH=%QT_PATH%"


set PACK_INFO_FILE=%CURRENT_DIR%\packet.ini
del /a /f "%PACK_INFO_FILE%"

set PACKET_TIME=%date:~0,4%/%date:~5,2%/%date:~8,2%-%time:~0,2%:%time:~3,2%:%time:~6,2%

echo [packet] > "%PACK_INFO_FILE%"


set PUBLISH_SHADER="%CURRENT_DIR%\..\windows\publish.bat"

set BUILD_ROOT_DIR=%BUILD_ROOT%\windows
set BUILD_SHADER="%CURRENT_DIR%..\windows\windows.build64.bat"
set CONFIG_PATH="%CURRENT_DIR%..\config\option-config-cmake\app.option.config.cmake"
echo "BUILD_ROOT_DIR : !BUILD_ROOT_DIR!"
echo "CONFIG_PATH : !CONFIG_PATH!"

echo "build project...."

echo "execute !BUILD_SHADER! ON !BUILD_ROOT_DIR! !CONFIG_PATH! !QT_PATH!"
call !BUILD_SHADER! ON !BUILD_ROOT_DIR! !CONFIG_PATH! !QT_PATH!
if not errorlevel 0 exit /b -1

echo "execute !BUILD_SHADER! OFF !BUILD_ROOT_DIR! !CONFIG_PATH! !QT_PATH!"
call !BUILD_SHADER! OFF !BUILD_ROOT_DIR! !CONFIG_PATH! !QT_PATH!
if not errorlevel 0 exit /b -1

echo "build project.... Finished"


del /a /f "%PACK_INFO_FILE%"

echo "execute %~dp0 build.bat success !"
exit /b 0