@echo off
setlocal enabledelayedexpansion

set BUILDER_CONFIG_PATH=%~dp0..\config\config.json

for /f "usebackq delims= " %%v in (`type %BUILDER_CONFIG_PATH% ^| python -c "import json; import sys; obj=json.load(sys.stdin); print(obj['project'], end='')"`) do ( set BUILDER_CONFIG_PROJECT=%%v)
for /f "usebackq delims= " %%v in (`type %BUILDER_CONFIG_PATH% ^| python -c "import json; import sys; obj=json.load(sys.stdin); print(obj['version'], end='')"`) do ( set BUILDER_CONFIG_VERSION=%%v)
for /f "usebackq delims= " %%v in (`type %BUILDER_CONFIG_PATH% ^| python -c "import json; import sys; obj=json.load(sys.stdin); print(obj['suffix'], end='')"`) do ( set BUILDER_CONFIG_SUFFIX=%%v)
for /f "usebackq delims= " %%v in (`type %BUILDER_CONFIG_PATH% ^| python -c "import json; import sys; obj=json.load(sys.stdin); print(obj['build'], end='')"`) do ( set BUILDER_CONFIG_BUILD=%%v)
for /f "usebackq delims= " %%v in (`type %BUILDER_CONFIG_PATH% ^| python -c "import json; import sys; obj=json.load(sys.stdin); print(obj['type'], end='')"`) do ( set BUILDER_BUILD_TYPE=%%v)
for /f "usebackq delims= " %%v in (`type %BUILDER_CONFIG_PATH% ^| python -c "import json; import sys; obj=json.load(sys.stdin); print(obj['author'], end='')"`) do ( set BUILDER_AUTHOR=%%v)

if "%BUILDER_AUTHOR%" == "" (
    set pyscript=%~dp0..\depends\pack_utils.py
    for /f "usebackq delims= " %%v in (`python !pyscript! author !BUILDER_CONFIG_PATH! `) do ( set BUILDER_AUTHOR=%%v)
)


echo "BUILDER_AUTHOR: %BUILDER_AUTHOR%"

set "BUILDER_CONFIG_PACK_VERSION_STR=%BUILDER_CONFIG_VERSION%-%BUILDER_BUILD_TYPE%-%BUILDER_CONFIG_BUILD%-%BUILDER_CONFIG_SUFFIX%"
set "BUILDER_CONFIG_USE_TAG=%BUILDER_CONFIG_VERSION%-%BUILDER_BUILD_TYPE%-%BUILDER_CONFIG_BUILD%-%BUILDER_CONFIG_SUFFIX%"

echo "BUILDER_CONFIG_PACK_VERSION_STR: %BUILDER_CONFIG_PACK_VERSION_STR%"
echo "BUILDER_CONFIG_USE_TAG: %BUILDER_CONFIG_USE_TAG%"
echo "remove the end of '-'"

rem 在延迟扩展的代码块中修改变量
rem 移除尾部的 - 字符
:remove_hyphen_a
if "!BUILDER_CONFIG_PACK_VERSION_STR:~-1!" equ "-" (
    set "BUILDER_CONFIG_PACK_VERSION_STR=!BUILDER_CONFIG_PACK_VERSION_STR:~0,-1!"
    goto remove_hyphen_a
)

rem 移除尾部的 - 字符
:remove_hyphen_b
if "!BUILDER_CONFIG_USE_TAG:~-1!" equ "-" (
    set "BUILDER_CONFIG_USE_TAG=!BUILDER_CONFIG_USE_TAG:~0,-1!"
    goto remove_hyphen_b
)

echo "BUILDER_CONFIG_PACK_VERSION_STR: %BUILDER_CONFIG_PACK_VERSION_STR%"
echo "BUILDER_CONFIG_USE_TAG: %BUILDER_CONFIG_USE_TAG%"

rem 禁用延迟扩展并设定变量，这样就可以在代码块外部使用变量了
endlocal & (
    set BUILDER_CONFIG_PACK_VERSION_STR=%BUILDER_CONFIG_PACK_VERSION_STR%
    set BUILDER_CONFIG_USE_TAG=%BUILDER_CONFIG_USE_TAG%
)

echo "endlocal........"
echo "BUILDER_CONFIG_PACK_VERSION_STR: %BUILDER_CONFIG_PACK_VERSION_STR%"
echo "BUILDER_CONFIG_USE_TAG: %BUILDER_CONFIG_USE_TAG%"



