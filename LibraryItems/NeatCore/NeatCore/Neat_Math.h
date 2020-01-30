/***************************************************************************************************
 Created on: 2020-01-27
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatCore Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_MATH_H_20200127__
#define __NEAT_MATH_H_20200127__

/**
 * This file defines some simple mathematical function interfaces.
 */
namespace neat
{
    /**
     * Returns 1 or -1 depending on the sign of a numeric value of type double
     */
    inline double Sign(double nValue_)
    {
        return nValue_ < 0.0 ? -1.0 : 1.0;
    }

    /**
     * Calculate the inner product of two vectors (dot product / quantity product)
     */
    inline double DotProduct(const DPOINT2& ptLeft_, const DPOINT2& ptRight_)
    {
        return ptLeft_[axis::x] * ptRight_[axis::x] + ptLeft_[axis::y] * ptRight_[axis::y];
    }

    inline double DotProduct(const DPOINT3& ptLeft_, const DPOINT3& ptRight_)
    {
        return ptLeft_[axis::x] * ptRight_[axis::x] + ptLeft_[axis::y] * ptRight_[axis::y] + ptLeft_[axis::z] * ptRight_[axis::z];
    }

    /**
     * Calculate the outer product of two vectors (cross product / vector product)
     */
    inline double CrossProduct(const DPOINT2& ptLeft_, const DPOINT2& ptRight_)
    {
        return ptLeft_[axis::x] * ptRight_[axis::y] - ptLeft_[axis::y] * ptRight_[axis::x];
    }

    inline DPOINT3 CrossProduct(const DPOINT3& ptLeft_, const DPOINT3& ptRight_)
    {
        return DPOINT3(ptLeft_[axis::y] * ptRight_[axis::z] - ptLeft_[axis::z] * ptRight_[axis::y],
            ptLeft_[axis::z] * ptRight_[axis::x] - ptLeft_[axis::x] * ptRight_[axis::z],
            ptLeft_[axis::x] * ptRight_[axis::y] - ptLeft_[axis::y] * ptRight_[axis::x]);
    }

    /**
     * Calculate the angle of rotation from the positive X-axis counter clockwise to the specified vector direction
     * in the two-dimensional Cartesian coordinate system, the angle range: [0, 2¦Ð)
     */
    double ComputeAngle(const DPOINT2& ptVector_);
    double ComputeAngle(double nX_, double nY_);

} // End namespace neat

#endif // __NEAT_MATH_H_20200127__