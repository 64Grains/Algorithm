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

void neat::RetrieveArcNode2D(const DPOINT2& ptStart_, const DPOINT2& ptEnd_, double nBulge_, ArcNode2D& ArcNode_)
{
    NEAT_RAISE(Precision::IsAlmostEqual(nBulge_, 0.0, Precision::RealTolerance()), g_strIllegalArcBulge);

    DPOINT2 _ptChordVector = ptEnd_ - ptStart_;
    double _nChordLength = _ptChordVector.GetLength();
    if (Precision::IsAlmostEqual(_nChordLength, 0.0, Precision::RealTolerance())) {
        // Arc with chord too short is treated as point
        ArcNode_.ptCenter = ptStart_;
        ArcNode_.nRadius = 0.0;
        ArcNode_.nStartAngle = 0.0;
        ArcNode_.nEndAngle = 0.0;
        return;
    }

    // Central angle
    double _nCentralAngle = 4.0 * atan(nBulge_);

    // Radius
    double _nRadius = 0.5 * _nChordLength / sin(_nCentralAngle * 0.5);
    ArcNode_.nRadius = fabs(_nRadius);

    // Center
    DPOINT2 _ptStretch = _ptChordVector * (_nRadius / _nChordLength);
    double _nRevAngle = (Precision::PIE() - _nCentralAngle) * 0.5;
    double _nCosAngle = cos(_nRevAngle);
    double _nSinAngle = sin(_nRevAngle);
    double _nDiffVectorX = _ptStretch[axis::x] * _nCosAngle - _ptStretch[axis::y] * _nSinAngle;
    double _nDiffVectorY = _ptStretch[axis::x] * _nSinAngle + _ptStretch[axis::y] * _nCosAngle;

    ArcNode_.ptCenter[axis::x] = ptStart_[axis::x] + _nDiffVectorX;
    ArcNode_.ptCenter[axis::y] = ptStart_[axis::y] + _nDiffVectorY;

    // Start angle
    ArcNode_.nStartAngle = ComputeAngle(ptStart_ - ArcNode_.ptCenter);

    // End angle
    ArcNode_.nEndAngle = ArcNode_.nStartAngle + _nCentralAngle;
}