@echo off & setlocal EnableDelayedExpansion

:: Solution directory
set abSolutionDir="%~dp0..\"
set abExcludeLib="gtest"

:: Copy the latest dependent libraries
call %~dp0..\..\..\Tools\AutoScript\GetLatestLibrary.bat %abSolutionDir% %abExcludeLib%
if %errorlevel% GTR 0 (
    echo ERROR: Failed to get dependent packages, please check for errors!
	exit /B 1
)

exit /B 0