@echo off
echo "begin publish SDK-Binary"

set current_dir=%~dp0%

cd %current_dir%

if not defined BRANCH_NAME (
	echo "define BRANCH_NAME"
	set BRANCH_NAME="main"
)

if %BRANCH_NAME%=="" (
	for /f "delims=" %%t in (
	'git branch --show-current'
	) do (
		set BRANCH_NAME=%%t
	)
)
echo "Found current branch name %BRANCH_NAME%"
set branch_name=%BRANCH_NAME%

set commit_hash=""
for /f "delims=" %%t in (
    'git rev-parse HEAD'
) do (
    set commit_hash=%%t
)

cd %current_dir%

echo Get git publish repository url
call %current_dir%/../depends/git.deploy.config.bat
call %current_dir%/../depends/config.bat

set PUBLISH_GIT=%PUBLISH_GIT_URL%
set PUBLISH_TAG=%BUILDER_CONFIG_USE_TAG%
echo "PUBLISH_GIT = %PUBLISH_GIT%"
echo "PUBLISH_TAG = %PUBLISH_TAG%"

set PUBLISH_NAME=%current_dir%/../build_app/publish/sdk_binary
if exist "%PUBLISH_NAME%" (
   echo "%PUBLISH_NAME% is exist"
) else (
	mkdir "%PUBLISH_NAME%"
  	git clone "%PUBLISH_GIT%" "%PUBLISH_NAME%"
)

if exist "%PUBLISH_NAME%" (
   echo "%PUBLISH_NAME% is exist"
) else (
   echo "%PUBLISH_NAME% is error"
   exit /b -1
)

cd "%PUBLISH_NAME%"
echo "cd %PUBLISH_NAME%"
git reset --hard origin/%branch_name%
git clean -xdf

set find_branch=""
for /f "delims=" %%t in (
	'git rev-parse --verify origin/%%branch_name%%'
) do (
	set find_branch=%%t
)

echo "find_branch = %find_branch%"

if %find_branch%=="" (
	echo "no find branch"
	git checkout -b %branch_name%
)  else (
    echo "find branch"
	git checkout %branch_name%
	git fetch
    git pull
)

echo "cp prebult..."
set source_dir=%current_dir%/../build_app/windows/install/windows
set dest_dir=%PUBLISH_NAME%/windows
 echo source_dir %source_dir%
 echo dest_dir %dest_dir%
if exist "%source_dir%" (
    echo source_dir exists.
) else (
    echo source_dir does not exist. %source_dir%
	exit /b -1
)

rem 先清空目标文件夹 
if exist "%dest_dir%" (
    echo dest_dir exists. 
	echo rmdir /s /q "%dest_dir%"
	rmdir /s /q "%dest_dir%"
)

mkdir "%dest_dir%"
if exist "%dest_dir%" (
    echo dest_dir exists.
) else (
    echo dest_dir does not exist. %dest_dir%
	exit /b -1
)

rem 拷贝文件
echo XCOPY /S /Y "%source_dir%" "%dest_dir%"
XCOPY /S /Y "%source_dir%" "%dest_dir%"

rem read tag_name from file
cd "%current_dir%"

set tag_name=%PUBLISH_TAG%

cd "%PUBLISH_NAME%"
echo "cd %PUBLISH_NAME%"
echo "tag_name=%tag_name%"

rem git添加文件并推送
git add -A
git commit -m "[apollo]: windows-pack %tag_name%"
git push -u origin %branch_name%

if %tag_name%=="" (
    echo "Git command returned failure (tag does not exist)"
    echo "Tag %tag_name% does not exist."
) else (
  rem 切换到publish仓库目录
  cd "%PUBLISH_NAME%"

rem 先判断tag是否存在，存在则删除
rem 检查标签是否存在
rem 以下tag删除和推送流程先屏蔽，推送tag统一在mac下执行-----------------
rem git show-ref --tags | findstr /C:"%tag_name%"
rem if %errorlevel% equ 0 (
rem    echo "Git command returned success (tag exists)"
rem    rem 如果存在，则删除标签
rem    git tag -d "%tag_name%"
rem    rem 同步删除操作到远程仓库（如果需要的话）
rem    git push --delete origin "%tag_name%"
rem    echo "Tag %tag_name% deleted."
rem  ) else (
rem      echo Tag %tag_name% does not exist.
rem  )
rem
rem  git tag -a "%tag_name%" -m "add tag %tag_name%"
rem  git push origin %tag_name%
rem -----------------
 )
cd %current_dir%