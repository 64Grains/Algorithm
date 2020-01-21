@echo off & setlocal EnableDelayedExpansion
echo ********** Build Solution NeatCore.sln **********

:: Solution file path
set abSolutionFile="%~dp0\NeatCore.sln"

call %~dp0\..\..\Tools\AutoScript\BuildAllConfig.bat %abSolutionFile%
if %errorlevel% GTR 0 (
	echo ERROR: Solution compilation failed, please check for errors!
	pause
	exit /B 1
)

echo.
pause
exit /B 0