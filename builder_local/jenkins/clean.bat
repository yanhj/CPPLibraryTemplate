@echo off

set CURRENT_DIR=%~dp0

rmdir /Q /S "%CURRENT_DIR%..\build_app"

git checkout .
git clean -df
git reset --hard
git submodule update --init --recursive

cd %CURRENT_DIR%..\..\third_party
%CURRENT_DIR%../../third_party/update_gitem_fast.bat
exit /b 0
::pause