@echo off & setlocal EnableDelayedExpansion
echo ********** Clean Solution NeatSpline.sln **********

:: Solution name + Solution directory
set abSolutionName="NeatSpline"
set abSolutionDir="%~dp0"

call %~dp0\..\..\Tools\AutoScript\CleanAllConfig.bat %abSolutionName% %abSolutionDir%

echo.
pause
exit /B 0