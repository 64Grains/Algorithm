@title Build Solution
@echo off & setlocal EnableDelayedExpansion

:: Usage: Batch script name + solution file

:: Here you need to set the system environment variable VSHOME first,
:: and the value is the IDE directory of Visual Studio.
set abDevenv="%VSHOME%\devenv.com"
set abSolutionFile="%~1"

:: Compiling solutions in various configurations
for %%i in ("Debug|x86" "Release|x86" "Debug|x64" "Release|x64") do (
    call :BuildSolution %%i || (exit /B 1)
)

exit /B 0

:: Compile the solution
:BuildSolution
set abSolutionConfig="%~1"
%abDevenv% %abSolutionFile% /Build %abSolutionConfig% || (exit /B 1)
exit /B 0