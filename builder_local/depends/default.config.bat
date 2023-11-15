@echo off
::setlocal enabledelayedexpansion

set CURRENT_DIR0=%~dp0

set PROJET_ROOT=%CURRENT_DIR0%..\..
set BUILD_ROOT=%CURRENT_DIR0%..\build_app
set DEFAULT_BUILD_ROOT_DIR="%BUILD_ROOT%\windows"
set DEFAULT_PACKET_ROOT_DIR="%BUILD_ROOT%\windows_packet"

exit /b 0
