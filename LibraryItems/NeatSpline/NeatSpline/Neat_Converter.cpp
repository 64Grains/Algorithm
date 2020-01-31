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
void neat::ConvertNurbsToBezier(const NurbsNode2D* pNurbsNode_, std::vector<BezierNode2D>& vecBezierNodes_)
{
    CNurbsConverter<DPOINT2, DPOINT3> _Converter2D;
    _Converter2D.ConvertNurbsToBezier(pNurbsNode_, vecBezierNodes_);
}

void neat::ConvertNurbsToBezier(const NurbsNode3D* pNurbsNode_, std::vector<BezierNode3D>& vecBezierNodes_)
{
    CNurbsConverter<DPOINT3, DPOINT4> _Converter3D;
    _Converter3D.ConvertNurbsToBezier(pNurbsNode_, vecBezierNodes_);
}