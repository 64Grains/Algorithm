/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_SPLINE_H_20200130__
#define __NEAT_SPLINE_H_20200130__

/**
 * Import and export symbols for dynamic libraries
 */
#ifdef NEATSPLINE_EXPORTS
    #define NEATSPLINE_API __declspec(dllexport)
#else
    #define NEATSPLINE_API __declspec(dllimport)
#endif

/**
 * This file contains all the interface files that are open to the public.
 * In order to facilitate external reference of this library, you only need to include this header file.
 */
#include "Spline_ErrorID.h"
#include "Neat_Parser.h"
#include "Neat_Scatter.h"
#include "Neat_Converter.h"
#include "Neat_Fitter.h"

#endif // __NEAT_SPLINE_H_20200130__