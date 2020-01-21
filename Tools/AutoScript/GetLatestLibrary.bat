@title Get Latest Library
@echo off & setlocal EnableDelayedExpansion

:: Usage: Batch script name + solution directory

set abSolutionDir=%~1

:: Pull the specified library package from the latest release library
set abLatestLibrary="%abSolutionDir%..\..\Publish\PublishVersion.txt"

:: Record the dependent library version in a file, and record the file in release package when packaging
set abNoteFile="%abSolutionDir%_Pub\DependLibs.txt"
set abFirstWrite=1

:: Read dependent library name and version from Readme.txt file
set abReadmeFile="%abSolutionDir%Readme.txt"
for /f "delims=: tokens=1*" %%i in ('Type %abReadmeFile% ^| findstr /n "Dependend:"') do (
    set n=&set /a n=%%i+1
)

for /f "tokens=1* delims=:" %%a in ('Type %abReadmeFile% ^| findstr /n .*') do (
    if %%a geq !n! (
	    if "%%b"=="" (
		    goto EndTag
		) else (
		    call :ParseNameVersion "%%b"
		)
	)
)

:EndTag
exit /B 0

:: Parse the library name and version from the string
:ParseNameVersion
for /f "tokens=1,*" %%i in ("%~1") do (
    if "%%i"=="None" (
	    goto EndTag
	)

	if %abFirstWrite% EQU 1 (
	    if not exist "%abSolutionDir%_Pub\" mkdir "%abSolutionDir%_Pub\"
		if exist %abNoteFile% del %abNoteFile%
		echo Dependend: > %abNoteFile%
		set abFirstWrite=0
	)

	:: Pulling dependent libraries
	for /f "tokens=1* delims=" %%j in ('Type %abLatestLibrary% ^| findstr /r "MapLibrary.*\"%%i\""') do (
	    call :MapLibrary "%%i" "%%j" || (exit /B 0)
	)
)
exit /B 0

:: Pull library package based on library name and version
:MapLibrary
set oldName="%~1"
set oldVersion="%~2"
set "oldVersion=%oldVersion:MapLibrary=%"
set "oldVersion=!oldVersion:%oldName%=!"
set "oldVersion=%oldVersion: =%"
set "oldVersion=%oldVersion:~1,-1%"
:: Record reference library and its version number
set newName=%oldName:~1,-1%
set newVersion=%oldVersion:~1,-1%
call :RecordLibrary %newName% %newVersion%
:: Copy and decompress the reference library of the corresponding version
if exist "%abSolutionDir%..\..\Publish\%newName%\%newName%-%newVersion%.7z" (
    call :UpdatePublish "%abSolutionDir%..\..\Publish\%newName%\%newName%-%newVersion%.7z" "%abSolutionDir%_Pub\%newName%" "%abSolutionDir%_Pub\%newName%_pub.h"
) else (
    call :ShowMessageERROR "%abSolutionDir%..\..\Publish\%newName%\%newName%-%newVersion%" || (exit /B 1)
)
exit /B 0

:: Record library and its version number
:RecordLibrary
set abName=%~1
set abVersion=%~2
set abNameLen=0
set abNameTemp=%abName%
:counter
if not "%abNameTemp%"=="" (
    set /a abNameLen+=1
	set "abNameTemp=%abNameTemp:~1%"
	goto counter
)
set abNameTemp=%abName%
:inserter
if %abNameLen% lss 30 (
    set /a abNameLen+=1
	set "abNameTemp=%abNameTemp% "
	goto inserter
)
echo    %abNameTemp%%abVersion% >> %abNoteFile%
exit /B 0

:: Copy and decompress library of the corresponding version
:UpdatePublish
if exist "%~f2\..\%~nx1" if exist "%~f2" if exist "%~f3" exit /B 0
xcopy "%~f1" "%~f2\..\" /c /h /r /y /f
if exist "%~f2" rmdir "%~f2" /S /Q
mkdir "%~f2"
pushd "%~f2"
call "%abSolutionDir%..\..\Tools\7za.exe" x -aoa "%~f2\..\%~nx1" > nul
popd
echo #include ".\%~n2\include\%~n2.h" > "%~f3"
exit /B 0

:: Show error message
:ShowMessageERROR
echo Important file was not found, missing file: %~1
exit /B 1