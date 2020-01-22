# Library Items

This directory stores the source code of all library projects.

## Project template

Here we default you to create a project according to the Visual Studio 2019 New Project Wizard and uncheck the option "Place solution and project in the same directory". If you want to create a unit test project, the name must be the solution name plus Tester.

Assuming the project name is `NestCore`, an expected complete solution directory structure is as follow

```
NeatCore (directory of solution)
    NeatCore (dynamic library or static library)
        NeatCore.cpp
        NeatCore.h
        NeatCore.vcxproj
        NeatCore.vcxproj.filters
        PreBuildEvent.bat
        ...
    NeatCoreTester (application)
        NeatCoreTester.cpp
        NeatCoreTester.vcxproj
        NeatCoreTester.vcxproj.filters
        ...
    BuildSolution.bat
    CleanSolution.bat
    NeatCore.sln
    PackSolution.bat
    Readme.txt
```

The `Readme.txt` file contains the library name, version number, names of dependent libraries, and update records.

The `PreBuildEvent.bat` file reads the name of the dependent library from the `Readme.txt` file, then pulls the latest version package from the publish folder. Then check if the dependent library version recorded in the publish package is the latest version in the `PublishVersion.txt` file.

The `BuildSolution.bat` file can compile various configurations of the solution without opening Visual Studio 2019.

The `CleanSolution.bat` file can clean up the temporary files compiled of the solution under each configuration without opening Visual Studio 2019.

The `PackSolution.bat` file can package the compiled library files, interface files and source files, and then place them in the publish directory.

## Project `NeatCore`

