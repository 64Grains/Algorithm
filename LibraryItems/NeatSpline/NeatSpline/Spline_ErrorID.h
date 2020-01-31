/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __SPLINE_ERRORID_H_20200130__
#define __SPLINE_ERRORID_H_20200130__

#include <string>

/**
 * This file defines the error message ID.
 * The external program can replace the description of the resource file based on the ID.
 */
namespace neat
{
    /**
     * Pointer is empty
     */
    const std::string g_strEmptyPointer = "ID_SPLINE_EMPTY_POINTER";

    /**
     * NURBS spline parameters error
     */
    const std::string g_strNurbsParamsError = "ID_SPLINE_NURBS_PARAMETERS_ERROR";

    /**
     * Wrong degree of nurbs spline
     */
    const std::string g_strWrongNurbsDegree = "ID_SPLINE_WRONG_NURBS_DEGREE";

    /**
     * Wrong deflection of nurbs spline
     */
    const std::string g_strWrongDeflection = "ID_SPLINE_WRONG_DEFLECTION";

    /**
     * Wrong dimension of Cartesian space
     */
    const std::string g_strWrongDimension = "ID_SPLINE_WRONG_DIMENSION";

    /**
     * Bezier spline parameters error
     */
    const std::string g_strBezierParamsError = "ID_SPLINE_BEZIER_PARAMETERS_ERROR";

    /**
     * Function is not implemented
     */
    const std::string g_strFunctionNotImplemented = "ID_SPLINE_FUNCTION_NOT_IMPLEMENTED";

    /**
     * Unknown error
     */
    const std::string g_strUnknownError = "ID_SPLINE_UNKNOWN_ERROR";

} // End namespace neat

#endif // __SPLINE_ERRORID_H_20200130__