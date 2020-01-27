/***************************************************************************************************
 Created on: 2020-01-27
 Created by: dquan2007@163.com
 Copyright (c) 2020-2027

 This file is part of C++ NeatCore Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#include "pch.h"

using namespace neat;

//////////////////////////////////////////////////////////////////////////
// global function
double neat::ComputeAngle(const DPOINT2& ptVector_)
{
    return ComputeAngle(ptVector_[axis::x], ptVector_[axis::y]);
}

double neat::ComputeAngle(double nX_, double nY_)
{
    double _nLength = sqrt(nX_ * nX_ + nY_ * nY_);
    if (Precision::IsAlmostEqual(_nLength, 0.0, Precision::RealTolerance())) {
        return 0.0;
    }

    double _nCosAngle = nX_ / _nLength;
    __CUT(_nCosAngle, -1.0, 1.0);

    double _nAngle = acos(_nCosAngle);
    if (nY_ < 0.0) {
        _nAngle = 2.0 * Precision::PIE() - _nAngle;
    }

    return _nAngle;
}