@echo off & setlocal EnableDelayedExpansion

:: Solution directory
set abSolutionDir="%~dp0..\"

:: Copy the latest dependent libraries
call %~dp0..\..\..\Tools\AutoScript\GetLatestLibrary.bat %abSolutionDir%
if %errorlevel% GTR 0 (
    echo ERROR: Failed to get dependent packages, please check for errors!
	exit /B 1
)

exit /B 0