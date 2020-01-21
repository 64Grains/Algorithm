@title Check dependent libraries
@echo off & setlocal EnableDelayedExpansion

:: Usage: Batch script name + module name + record file path + latest libraries file path
:: @record file path: This file comes form the release library, and records the name and version of the library that the release library depends on
:: @latest libraries file path: This file records the name and version of the published library

set abModuleName=%~1
set abRecordFile="%~2"
set abLatestLibrary="%~3"

:: Check whether the version of the dependent library recorded in the release package is consistent with the latest library version currently released
set /A abNotSameVersion=0
for /f "delims=: tokens=1*" %%i in ('Type %abRecordFile% ^| findstr /n "Dependend:"') do (
    set n=&set /a n=%%i+1
)

for /f "tokens=1* delims=:" %%a in ('Type %abRecordFile% ^| findstr /n .*') do (
    if %%a geq !n! (
        if "%%b"=="" (
            goto EndTag
        ) else (
            call :ParseNameVersion "%%b" || (exit /B 1)
        )
    )
)

:EndTag
exit /B %abNotSameVersion%

:: Parse the libraries name and version from the string
:ParseNameVersion
for /f "tokens=1,*" %%i in ("%~1") do (
    if "%%i"=="None" (
        goto EndTag
    )

    call :MatchLibrary "%%i" "%%j" || (exit /B 1)
)
exit /B 0

:: Match library version based on library name
:MapLibrary
set abLibName="%~1"
set abLibVersion="%~2"
for /f "tokens=1* delims=" %%j in ('Type %abLatestLibrary% ^| findstr /r "MapLibrary.*\"%abLibName%\""') do (
    call :MatchVersion %abLibName% %abLibVersion% "%%j" || (exit /B 1)
)
exit /B 0

:: Match library version
:MatchVersion
set abLibName="%~1"
set abRecordVersion="%~2"
set "abRecordVersion=%abRecordVersion: =%"
set abLatestVersion="%~3"
set "abLatestVersion=%abLatestVersion:MapLibrary=%"
set "abLatestVersion=!abLatestVersion:%abLibName%=!"
set "abLatestVersion=%abLatestVersion: =%"
set "abLatestVersion=%abLatestVersion:~1,-1%"
if "%abRecordVersion%" EQU "%abLatestVersion%" exit /B 0
echo "%abModuleName% does not reference the latest release library: %abLibName:~1,-1%"
set /A abNotSameVersion=%abNotSameVersion%+1
exit /B 1