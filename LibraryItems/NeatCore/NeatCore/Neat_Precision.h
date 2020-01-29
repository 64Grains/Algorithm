/***************************************************************************************************
 Created on: 2020-01-27
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatCore Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_PRECISION_H_20200127__
#define __NEAT_PRECISION_H_20200127__

#include <cmath>

/**
 * This file defines the numerical precision of double precision floating point numbers.
 */
namespace neat
{
    /**
     * The Precision class provides a set of functions that define the standard of precision used when comparing two double
     * values under normal circumstances.
     * It is not recommended to directly use == to compare two floating point numbers. When comparing floating point numbers.
     * you should judge within the given accuracy range.
     * In addition, when comparing floating point numbers, you should avoid the "big numbers eat decimals" problem, such as:
     * double Precision = ...
     * double x1 = ...    ! a large number
     * double x2 = ...    ! another large number
     * if ( x1 < x2 - Precision ) ...
     * The above use methods should be avoided, it is best to use as follows:
     * if ( x2 - x1 > Precision ) ...
     */
    class Precision
    {
    public:
        /**
         * Use this precision when comparing two double precsion floating point values, such as:
         * double x1 = ..., x2 = ...;
         * if ( fabs( x2 - x1 ) < Precision::RealPrecision() ) ...
         */
        static double RealTolerance() { return 1.0e-9; }
        static double SquareRealTolerance() { return RealTolerance() * RealTolerance(); }

        /**
         * Comparing two double precision floating point numbers are equal within a certain precision range
         */
        static bool IsAlmostEqual(double nLeft_, double nRight_, double nTolerance_)
        {
            return fabs(nLeft_ - nRight_) <= nTolerance_;
        }

        /**
         * Pi ¦Ð
         */
        static double PIE() { return 3.1415926535897932384626; }

        /**
         * Invalid double precision floating point number
         */
        static double InvalidDouble() { return std::nan(""); }
        static bool IsInvalidDouble(double nNumber_) { return std::isnan(nNumber_); }

        /**
         * Infinite double precision floating point number
         */
        static double InfinityDouble() { return INFINITY; }
        static bool IsInfinityDouble(double nNumber_) { return std::isinf(nNumber_); }
    };

} // End namespace neat

#endif // __NEAT_PRECISION_H_20200127__