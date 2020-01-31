/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#include "pch.h"

using namespace neat;

//////////////////////////////////////////////////////////////////////////
// global function interfaces
void neat::ScatterNurbs(const NurbsNode2D* pNurbsNode_, double nDeflection_, VECDPOINT2& vecScatterPoints_)
{
    CNurbsScatter<DPOINT2, DPOINT3> _Scatter2D;
    _Scatter2D.ScatterNurbs(pNurbsNode_, nDeflection_, vecScatterPoints_);
}

void neat::ScatterNurbs(const NurbsNode2D* pNurbsNode_, double nDeflection_, std::vector<ScatterNode2D>& vecScatterNodes_)
{
    CNurbsScatter<DPOINT2, DPOINT3> _Scatter2D;
    _Scatter2D.ScatterNurbs(pNurbsNode_, nDeflection_, vecScatterNodes_);
}

void neat::ScatterNurbs(const NurbsNode3D* pNurbsNode_, double nDeflection_, VECDPOINT3& vecScatterPoints_)
{
    CNurbsScatter<DPOINT3, DPOINT4> _Scatter3D;
    _Scatter3D.ScatterNurbs(pNurbsNode_, nDeflection_, vecScatterPoints_);
}

void neat::ScatterNurbs(const NurbsNode3D* pNurbsNode_, double nDeflection_, std::vector<ScatterNode3D>& vecScatterNodes_)
{
    CNurbsScatter<DPOINT3, DPOINT4> _Scatter3D;
    _Scatter3D.ScatterNurbs(pNurbsNode_, nDeflection_, vecScatterNodes_);
}

void neat::ScatterBezier(const BezierNode2D* pBezierNode_, double nDeflection_, VECDPOINT2& vecScatterPoints_)
{
    NurbsNode2D _NurbsNode2D;
    ConvertBezierToNurbs(pBezierNode_, &_NurbsNode2D);

    CNurbsScatter<DPOINT2, DPOINT3> _Scatter2D;
    _Scatter2D.ScatterNurbs(&_NurbsNode2D, nDeflection_, vecScatterPoints_);
}

void neat::ScatterBezier(const BezierNode2D* pBezierNode_, double nDeflection_, std::vector<ScatterNode2D>& vecScatterNodes_)
{
    NurbsNode2D _NurbsNode2D;
    ConvertBezierToNurbs(pBezierNode_, &_NurbsNode2D);

    CNurbsScatter<DPOINT2, DPOINT3> _Scatter2D;
    _Scatter2D.ScatterNurbs(&_NurbsNode2D, nDeflection_, vecScatterNodes_);
}

void neat::ScatterBezier(const BezierNode3D* pBezierNode_, double nDeflection_, VECDPOINT3& vecScatterPoints_)
{
    NurbsNode3D _NurbsNode3D;
    ConvertBezierToNurbs(pBezierNode_, &_NurbsNode3D);

    CNurbsScatter<DPOINT3, DPOINT4> _Scatter3D;
    _Scatter3D.ScatterNurbs(&_NurbsNode3D, nDeflection_, vecScatterPoints_);
}

void neat::ScatterBezier(const BezierNode3D* pBezierNode_, double nDeflection_, std::vector<ScatterNode3D>& vecScatterNodes_)
{
    NurbsNode3D _NurbsNode3D;
    ConvertBezierToNurbs(pBezierNode_, &_NurbsNode3D);

    CNurbsScatter<DPOINT3, DPOINT4> _Scatter3D;
    _Scatter3D.ScatterNurbs(&_NurbsNode3D, nDeflection_, vecScatterNodes_);
}