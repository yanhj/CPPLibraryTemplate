@echo off

setlocal

	set CURRENT_DIR=%~dp0%

 	echo /////////////////////////////////begin check git/////////////////////////////////
	cd %CURRENT_DIR%modules
	call gitem update --no-force-update

	echo /////////////////////////////////end  check git/////////////////////////////////
endlocal

exit /b 0
