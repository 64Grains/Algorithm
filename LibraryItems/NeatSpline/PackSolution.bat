@echo off & setlocal EnableDelayedExpansion
echo ********** Pack Solution NeatSpline.sln **********

:: Solution name + solution directory + Interface filter + library type
set abSolutionName="NeatSpline"
set abSolutionDir="%~dp0"
set abInterfaceFilter="Interface"
set abLibType=1

call %~dp0\..\..\Tools\AutoScript\PackAllConfig.bat %abSolutionName% %abSolutionDir% %abInterfaceFilter% %abLibType%
if %errorlevel% GTR 0 (
    echo ERROR: Packaging failed, please check for errors!
	pause 
	exit /B 1
)

echo.
pause
exit /B 0