@echo off & setlocal EnableDelayedExpansion
echo ********** Pack Solution NeatCore.sln **********

:: Solution name + solution directory + Interface filter + library type
set abSolutionName="NeatCore"
set abSolutionDir="%~dp0"
set abInterfaceFilter="Interface"
set abLibType=0

call %~dp0\..\..\Tools\AutoScript\PackAllConfig.bat %abSolutionName% %abSolutionDir% %abInterfaceFilter% %abLibType%
if %errorlevel% GTR 0 (
    echo ERROR: Packaging failed, please check for errors!
	pause 
	exit /B 1
)

echo.
pause
exit /B 0