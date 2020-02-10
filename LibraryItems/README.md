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

This library defines the basic data types and mathematical function interfaces. They will be introduced separately below.

### Numerical precision

The Precision class provides a set of functions that define the standard of precision used when comparing two double values under normal circumstances.

It is not recommended to directly use == to compare two floating point numbers. When comparing floating point numbers, you should judge within the given accuracy range. 

- `Precision::RealTolerance()`

Use this precision when comparing two double precision floating values, such as:

```c++
double x1 = ..., x2 = ...;
if ( fabs( x2 - x1 ) < Precision::RealTolerance() ) ...
```

- `Precision::SquareRealTolerance()`

Use this precision when comparing the square of two double precision floating values, such as:

```c++
double x1 = ..., x2 = ...;
if ( fabs( x2 * x2 - x1 * x1) < Precision::SquareRealTolerance() )
```

- `Precision::IsAlmostEqual(...)`

This function is used to judge whether two specified double values are equal within a given numerical precision range, such as:

```c++
double tolerance = ...;
double x1 = ..., x2 = ...;
if (Precision::IsAlmostEqual( x1, x2, tolerance )) ...
```

- `Precision::PIE()`

We all know that the pi is an infinite non-recurring decimal, here we use a finite decimal instead.

- `Precision::InvalidDouble()`

This function returns an invalid double value.

- `Precision::IsInvalidDouble(...)`

This function judges whether a double value is an invalid double value.

- `Precision::InfinityDouble()`

This function returns an infinite double value.

- `Precision::IsInfinityDouble(...)`

This function judges whether a double value is an infinite double value.

### Basic macro definition

- `max`

This macro calculates the maximum of two values.

- `min`

This macro calculates the minimum of two values.

- `max3`

This macro calculates the maximum of three values.

- `min3`

This macro calculates the minimum of three values.

- `__CUT`

This macro limits the specified value to the specified range.

It is assumed here that the first parameter value of the __CUT macro is smaller than the second parameter value, otherwise the result is unpredictable.

- `NEAT_RAISE_IF`

An exception macro with a switch. When the switch is on, it is exactly the same as `NEAT_RAISE`. When the switch is off, it means nothing.

You can switch this condition macro according to the macro definition. We tend to turn this macro off when  compiling the Release version in order to improve the efficiency of the program, but there is a risk that the exception is not thrown in time, and the program's behavior may unknowable.

- `NEAT_RAISE`

An exception macro. This macro contains two parameters, and the second parameter is the exception information. When the first parameter is true, this macro will throw std::domain_error exception with the exception information. 

### Basic data structure definition

- `struct axis`

This structure defines the index in the Cartesian space coordinate system, and the macro is used to replace the value to represent the index, which can make the code more readable. Examples are as follows

```c++
axis::x == 0
axis::y == 1
axis::z == 2
```

- `class CAxisPoint`

This class is a template class that can be used to represent coordinates or vectors in the Cartesian space coordinate system. It supports four arithmetic and subscript arithmetic.

- `class CFastVector`

A custom fast container of type std::vector. This container retains the elements in std::vector and controls the state of the fast container by manipulating the index.

- `struct NurbsNode`

This structure is a template for a nurbs spline.

- `struct BezierNode`

This structure is a template for a bezier spline.

- `ScatterNode`

This structure is a template used to store the scatter node information (including knot value, point coordinate, first-order derivative, second-order derivative) of a spline curve.

- `struct ArcNode2D`

This structure defines the arc node in the two-dimensional Cartesian coordinate system, which includes circle center, radius, start angle and end angle.

- `struct Polyline2D`

This structure defines the polyline in the two-dimensional Cartesian coordinate system, which includes start point coordinate and node sequence. The node in polyline is a line or an arc. If the bulge is zeros, it represents a line, otherwise it represents an arc.

The bulge of an arc is defined as the tan value of a quarter of the central angle. if the arc is counterclockwise, the central angle is positive, otherwise it is negative.

### Basic mathematical function definition

- `Sign`

This function returns a numeric sign, if it is negative, it returns -1, otherwise it returns 1.

- `DotProduct`

This function calculates the inner product of two vectors. 

This function is an overloaded function that can calculate the inner product in two-dimension and the inner   product in three-dimension.

- `CrossProduct`

This function calculates the outer product of two vectors.

This function is an overloaded function than can calculate the outer product in two-dimension and the outer product in three-dimension.

- `ComputeAngle`

This function calculates the angle of rotation from the positive x-axis counter clockwise to the specified vector direction in the two-dimensional Cartesian coordinate system. The angle range is [0, 2π).

This function is an overloaded function that allows vector coordinate to be passed in separately as function parameters.

- `RetrieveArcNode2D`

This function retrieves arc information in the two-dimensional Cartesian coordinate system based on start point, end point and bulge.

## Project `NeatSpline`

This library defines the parser, scatter, converter and fitter for splines. They will be introduced separately below.

### Spline parser

- `class CNurbsParser1D`

This class is the parser for one-dimensional nurbs spline, which can calculate the point coordinate, first-order and second-order derivatives at the specified knot.

- `class CNurbsParser2D`

This class is the parser for two-dimensional nurbs spline, which can calculate the point coordinate, first-order and second-order derivatives at the specified knot.

- `class CNurbsParser3D`

This class is the parser for three-dimensional nurbs spline, which can calculate the point coordinate, first-order and second-order derivatives at the specified knot.

- `class CBezierParser2D`

This class is the parser for two-dimensional bezier spline, which can calculate the point coordinate, first-order and second-order derivatives at the specified knot.

- `class CBezierParser3D`

This class is the parser for three-dimensional bezier spline, which can calculate the point coordinate, first-order and second-order derivatives at the specified knot.

### Spline scatter

- `ScatterNurbs`

This function is an overloaded function than can scatter two-dimensional nurbs spline into scattered points, scattered nodes, or polyline with the specified precision, and can scatter three-dimensional nurbs spline into scattered points or scattered nodes with the specified precision.

- `ScatterBezier`

This function is an overloaded function than can scatter two-dimensional bezier spline into scattered points, scattered nodes, or polyline with the specified precision, and can scatter three-dimensional bezier spline into scattered points or scattered nodes with the specified precision.

### Spline converter

- `ConvertNurbsToBezier`

This function converts the nurbs spline to bezier splines.

This function is an overloaded function than can convert two-dimensional nurbs spline and three-dimensional nurbs spline.

### Spline fitter

- `CNurbsInterp1D`

This class is the interpolator for one-dimensional nurbs spline, which can generate nurbs spline by interpolating fit points based on given conditions (start tangent vector, end tangent vector, knot vector, but these are not necessary).

- `CNurbsInterp2D`

This class is the interpolator for two-dimensional nurbs spline, which can generate nurbs spline by interpolating fit points based on given conditions (start tangent vector, end tangent vector, knot vector, but these are not necessary).

- `CNurbsInterp3D`

This class is the interpolator for three-dimensional nurbs spline, which can generate nurbs spline by interpolating fit points based on given conditions (start tangent vector, end tangent vector, knot vector, but these are not necessary).

