/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __PCH_H_20200130__
#define __PCH_H_20200130__

/**
 * This is a precompiled header file.
 * Files listed below are compiled only once, improving build performance for future builds.
 * This also affects IntelliSense performance, including code completion and many code browsing features.
 * However, files listed here are ALL re-compiled if any one of them is updated between builds.
 * Do not add files here that you will be updating frequently as this negates the performance advantage.
 */

// add headers that you want to pre-compile here
#include <windows.h>
#include "NeatSpline.h"

#include "NurbsPublic.h"
#include "NRBSplineParser.h"
#include "RBSplineParser.h"
#include "RBSplineParser2.h"
#include "Nurbs_Parser.h"
#include "NurbsRefiner.h"
#include "NurbsDeflection.h"
#include "Nurbs_Scatter.h"
#include "Nurbs_Converter.h"

#endif // __PCH_H_20200130__
