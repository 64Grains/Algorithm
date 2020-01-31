@title Pack Solution
@echo off & setlocal EnableDelayedExpansion

:: Usage: Batch script name + solution name + solution directory + Interface filter + library type
:: @Interface filter: This variable is used to filter the interface files to the include directory of the packaged file
:: @library type: 0 of the variable means static library, other values means dynamic library

set abSolutionName=%~1
set abSolutionDir=%~2
set abInterfaceFilter=%~3
set abLibType=%~4

:: Perform unit tests before packaging
set abIsRunTester=1
set abSolutionTestDir=%abSolutionDir%%abSolutionName%Tester\
if not exist "%abSolutionTestDir%" set abIsRunTester=0
if %abIsRunTester% EQU 1 (
    call :RunTester || (exit /B 1)
)

:: Generate package file version
set abVersion=0.000.0.0
set abReadVersion=
for /F "tokens=2 delims=: " %%i in ('type "%abSolutionDir%Readme.txt" ^| findstr "Version: "') do (
    set abReadVersion=%%i
)

if not "%abReadVersion%" == "" (
    set abVersion=%abReadVersion%
)

:: Generate package file path
set abPackPath=%abSolutionDir%..\..\Publish\%abSolutionName%\%abSolutionName%-%abVersion%\
if not exist "%abPackPath%" mkdir "%abPackPath%"

:: Check the compiled library file and copy it to the lib directory of the package path
for %%i in (Debug Release) do (
    for %%j in (Win32 x64) do (
        for %%k in (v142) do (
            for %%m in (lib pdb) do (
                if not exist "%abSolutionDir%%%i\%%j\%%k\%abSolutionName%.%%m" (
                    call :ShowMessageERROR "%abSolutionDir%%%i\%%j\%%k\%abSolutionName%.%%m"
                    exit /B 1
                )

                xcopy "%abSolutionDir%%%i\%%j\%%k\%abSolutionName%.%%m" "%abPackPath%lib\%%i\%%j\%%k\" /D /Q /Y
            )
			
            :: If it is a dynamic library, you also need to check the dll file
            if %abLibType% NEQ 0 (
                if not exist "%abSolutionDir%%%i\%%j\%%k\%abSolutionName%.dll" (
                    call :ShowMessageERROR "%abSolutionDir%%%i\%%j\%%k\%abSolutionName%.dll"
                    exit /B 1
                )

                xcopy "%abSolutionDir%%%i\%%j\%%k\%abSolutionName%.dll" "%abPackPath%lib\%%i\%%j\%%k\" /D /Q /Y
            )
        )
    )
)

:: Copy the interface file to the include directory of the package path
set abProjectDir=%abSolutionDir%%abSolutionName%
set abProjectFilter=%abProjectDir%\%abSolutionName%.vcxproj.filters
set abHeaderFile=
set abFlag=0
for /f "delims=" %%i in (%abProjectFilter%) do (
    echo "%%i" | findstr /c:"Include=">nul && set abFlag=1
    if !abFlag! EQU 1 (
        set abTemp=%%i
        set abTemp=!abTemp:*"=%!
        set abTemp=!abTemp:~0,-2!
        set abHeaderFile=!abTemp!
    )
	
    echo "%%i" | findstr /c:"<Filter>!abInterfaceFilter!">nul && set abFlag=2
    if !abFlag! EQU 2 (
        if not exist "!abProjectDir!\!abHeaderFile!" (
            call :ShowMessageERROR "!abProjectDir!\!abHeaderFile!"
            exit /B 1
        )
		
        xcopy "!abProjectDir!\!abHeaderFile!" "!abPackPath!include\" /D /Q /Y
        set abHeaderFile=
    )
	
    set abFlag=0
)

:: Copy the source file to the src directory of the package path
for %%i in (.h .c .rc) do (
    if exist "%abProjectDir%\*%%i*" (
        xcopy "%abProjectDir%\*%%i*" "%abPackPath%src\" /D /Q /Y
    )
)

:: Copy the Readme.txt file
xcopy "%abSolutionDir%Readme.txt" "%abPackPath%" /D /Q /Y

:: Copy dependent library version
if exist "%abSolutionDir%_Pub\DependLibs.txt" (
    xcopy "%abSolutionDir%_Pub\DependLibs.txt" "%abPackPath%" /D /Q /Y
)

:: Generate a compressed package
set abDestPath=%abSolutionDir%..\..\Publish\%abSolutionName%
set abDestDir=%abPackPath%
set abDestZip=%abDestPath%\%abSolutionName%-%abVersion%.7z
set abDest7za=%abSolutionDir%..\..\Tools\7za.exe
if exist "%abDestZip%" del /F /Q "%abDestZip%"

pushd "%abDestPath%"
"%abDest7za%" a "%abDestZip%" %abDestDir%*
popd
if exist "%abDestDir%" rd /S /Q "%abDestDir%"
echo Packaging is successful, details: "%abDestZip%"
exit /B 0

:: Perform unit tests
:RunTester
echo Performing unit tests...
for %%i in (Debug Release) do (
    for %%j in (Win32 x64) do (
        for %%k in (v142) do (
            if not exist "%abSolutionDir%%%i\%%j\%%k\%abSolutionName%Tester.exe" (
                call :ShowMessageERROR "%abSolutionDir%%%i\%%j\%%k\%abSolutionName%Tester.exe"
                exit /B 1
            )
			
            call %abSolutionDir%%%i\%%j\%%k\%abSolutionName%Tester.exe || (
                echo Unit test failed!
                echo.
                exit /B 1
            )
        )
    )
)
echo Unit tests run successfully!
echo.
exit /B 0

:: Show error message
:ShowMessageERROR
echo Important file was not found, missing file: %~1
exit /B 1