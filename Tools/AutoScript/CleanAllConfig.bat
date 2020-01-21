@title Clean Solution
@echo off & setlocal EnableDelayedExpansion

:: Usage: Batch script name + solution name + solution directory

set abSolutionName=%~1
set abSolutionDir=%~2

:: Clean up compiled temporary files
for %%i in (%abSolutionName% %abSolutionName%Tester) do (
    call :DetectAndDelete "%abSolutionDir%%%i\%%i.vcxproj.user"
    call :DetectAndDelete "%abSolutionDir%%%i\%%i.aps"
)

call :RmdirAndReport "%abSolutionDir%.vs\"
call :RmdirAndReport "%abSolutionDir%_Pub\"
call :RmdirAndReport "%abSolutionDir%Debug\"
call :RmdirAndReport "%abSolutionDir%Release\"

exit /B 0

:: Delete a file
:DetectAndDelete
if exist "%~1" (
    echo Deleting : %~1
    del /Q "%~1"
)
exit /B 0

:: Delete a folder
:RmdirAndReport
if exist "%~1" (
    echo Deleting : %~1
    rmdir /S /Q "%~1"
)
exit /B 0