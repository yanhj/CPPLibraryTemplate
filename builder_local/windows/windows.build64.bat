@echo off
setlocal enabledelayedexpansion

set CURRENT_DIR=%~dp0
call %CURRENT_DIR%..\depends\default.config.bat

rem 获取绝对路径
set BUILD_TYPE=Release
set BUILD_NAME=x64-%BUILD_TYPE%
set BUILD_GENERATOR="Visual Studio 15 2017 Win64"
set BUILD_COMMAND_ARGS="-m:8 -v:minimal -p:PreferredToolArchitecture=x64"
set CMAKE_COMMAND_ARGS=""
			  
set BUILD_SHARED_LIBS=%1
set BUILD_ROOT_DIR=%2
set CONFIG_PATH=%3

echo "BUILD_SHARED_LIBS=%BUILD_SHARED_LIBS%"
if %BUILD_ROOT_DIR% == "" (
	set BUILD_ROOT_DIR=%DEFAULT_BUILD_ROOT_DIR%
)

if %BUILD_ROOT_DIR% == "" (
	echo "error: invalid BUILD_ROOT_DIR"
	exit
)

mkdir %BUILD_ROOT_DIR%

rem 获取vs安装目录下的msbuild路径
set VSWHERE_EXE=%CURRENT_DIR%..\depends\vswhere.exe
for /f "usebackq tokens=1* delims=: " %%i in (`%VSWHERE_EXE% -latest -requires Microsoft.Component.MSBuild`) do (
  if /i "%%i"=="installationPath" set VS_INSTALL_DIR=%%j
)


echo ======================visual-studio-version====================
%VSWHERE_EXE% | find /i "displayName"
echo ======================visual-studio-version====================

::if the vs version grather than 2017 use 'Current' replace '15.0'
:: set MSBUILD_EXE=%VS_INSTALL_DIR%\MSBuild\Current\Bin\MSBuild.exe

set MSBUILD_EXE=%VS_INSTALL_DIR%\MSBuild\15.0\Bin\MSBuild.exe
echo MSBUILD_EXE=%MSBUILD_EXE%
rem 替换单斜杠为双斜杠
set MSBUILD_EXE=!MSBUILD_EXE:\=/!   
echo MSBUILD_EXE=%MSBUILD_EXE%
IF exist "%MSBUILD_EXE%" (
echo MSBUILD_EXE exist
) ELSE (
    echo "MSBUILD_EXE does not exist."
    exit /b -1
)

REM 获取当前盘符，然后切换到c盘
set CURRENT_HARDDISK=%~d0
C:
for /F %%i in ('echo %time:~0,2%-%time:~3,2%-%time:~6,2%') do (
	set CURRENT_TIME=%%i
)
set MKLINK_ROOT=C:\%CURRENT_TIME%
mkdir %MKLINK_ROOT%
set PROJET_ROOT_MKLINK=%MKLINK_ROOT%\Project
set BUILD_ROOT_DIR_MKLINK=%MKLINK_ROOT%\Build
cmd /c mklink /d %PROJET_ROOT_MKLINK% %PROJET_ROOT%
cmd /c mklink /d %BUILD_ROOT_DIR_MKLINK% %BUILD_ROOT_DIR%

set PROJET_ROOT=%PROJET_ROOT_MKLINK%
set BUILD_ROOT_DIR=%BUILD_ROOT_DIR_MKLINK%

set BUILD_SUB_DIR=%BUILD_ROOT_DIR%\build
set INSTALL_DIR=%BUILD_ROOT_DIR%\install
echo PROJET_ROOT=%PROJET_ROOT%
echo BUILD_SUB_DIR=%BUILD_SUB_DIR%
echo INSTALL_DIR=%INSTALL_DIR%


rem	删除上一次的打包目录
if exist %BUILD_SUB_DIR% (
	echo "rm %BUILD_SUB_DIR%"
	rmdir /Q /S "%BUILD_SUB_DIR%"
)
mkdir "%BUILD_SUB_DIR%"
cd "%BUILD_SUB_DIR%"

set QT_HOME_OPTION=-DCMAKE_QT_HOME_DEBUG=ON -DCMAKE_QT_HOME_PATH=%QT_PATH% -DBUILD_SHARED_LIBS=%BUILD_SHARED_LIBS%

rem 生成VS工程
cmake -G %BUILD_GENERATOR%                                  ^
      -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%"                ^
      -DCMAKE_BUILD_TYPE="%BUILD_TYPE%"                     ^
      -DBUILDER_PROJECT_CONFIG_FILE_PATH="%CONFIG_PATH%"    ^
      %QT_HOME_OPTION%                                      ^
      "%PROJET_ROOT%"

if %ERRORLEVEL% neq 0 exit /b -1

REM build Release
"%MSBUILD_EXE%" "%BUILD_SUB_DIR%\CustomLib.sln" /p:Configuration=%build_type%
if %ERRORLEVEL% neq 0 exit /b -1

"%MSBUILD_EXE%" "%BUILD_SUB_DIR%\INSTALL.vcxproj" /p:Configuration=%build_type%
if %ERRORLEVEL% neq 0 exit /b -1
						
REM build debug
"%MSBUILD_EXE%" "%BUILD_SUB_DIR%\CustomLib.sln" /p:Configuration=Debug
if %ERRORLEVEL% neq 0 exit /b -1

"%MSBUILD_EXE%" "%BUILD_SUB_DIR%\INSTALL.vcxproj" /p:Configuration=Debug
if %ERRORLEVEL% neq 0 exit /b -1

rmdir /Q /S "%MKLINK_ROOT%"
REM 还原到原来的盘符及路径
%CURRENT_HARDDISK%
cd %CURRENT_DIR%

exit /b 0
