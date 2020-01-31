/***************************************************************************************************
 Created on: 2020-01-31
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
// class CNurbsInterp
template<>
double CNurbsInterp<double>::GetLength(const double& ptLeft_, const double& ptRight_) const
{
    NEAT_RAISE(true, g_strFunctionNotImplemented);
}

template<>
double CNurbsInterp<DPOINT2>::GetLength(const DPOINT2& ptLeft_, const DPOINT2& ptRight_) const
{
    return (ptLeft_ - ptRight_).GetLength();
}

template<>
double CNurbsInterp<DPOINT3>::GetLength(const DPOINT3& ptLeft_, const DPOINT3& ptRight_) const
{
    return (ptLeft_ - ptRight_).GetLength();
}