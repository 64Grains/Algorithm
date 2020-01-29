/***************************************************************************************************
 Created on: 2020-01-29
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatCore Library tester application.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_CORE_TESTER_H_20200129__
#define __NEAT_CORE_TESTER_H_20200129__

/**
 * Type definition
 */
typedef neat::CAxisPoint<double, 4> DPOINT4;
typedef neat::CAxisPoint<double, 5> DPOINT5;
typedef neat::CAxisPoint<double, 6> DPOINT6;
typedef neat::CAxisPoint<int, 2> IPOINT2;
typedef neat::CAxisPoint<int, 3> IPOINT3;
typedef neat::CAxisPoint<bool, 2> BPOINT2;
typedef neat::CAxisPoint<bool, 3> BPOINT3;

/**
 * Check if two-dimensional coordinates are equal
 */
void CheckDpoint2Equal(const neat::DPOINT2& ptLeft_, const neat::DPOINT2& ptRight_, double nTolerance_);

/**
 * Check if three-dimensional coordinates are equal
 */
void CheckDpoint3Equal(const neat::DPOINT3& ptLeft_, const neat::DPOINT3& ptRight_, double nTolerance_);

#endif // __NEAT_CORE_TESTER_H_20200129__