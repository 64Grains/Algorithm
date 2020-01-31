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
// class CNurbsRefiner
template<>
void CNurbsRefiner<DPOINT2, DPOINT3>::UpgradePoles(const DPOINT2& ptPole_, double nWeight_, DPOINT3& ptHomogeneousPole_) const
{
    NEAT_RAISE_IF(DOUBLE_EQU_ZERO(nWeight_), g_strNurbsParamsError);
    ptHomogeneousPole_[axis::x] = ptPole_[axis::x] * nWeight_;
    ptHomogeneousPole_[axis::y] = ptPole_[axis::y] * nWeight_;
    ptHomogeneousPole_[axis::z] = nWeight_;
}

template<>
void CNurbsRefiner<DPOINT2, DPOINT3>::DowngradePoles(const DPOINT3& ptHomogeneousPole_, DPOINT2& ptPole_, double& nWeight_) const
{
    NEAT_RAISE_IF(DOUBLE_EQU_ZERO(ptHomogeneousPole_[axis::z]), g_strNurbsParamsError);
    double _nReciprocal = 1.0 / ptHomogeneousPole_[axis::z];
    ptPole_[axis::x] = ptHomogeneousPole_[axis::x] * _nReciprocal;
    ptPole_[axis::y] = ptHomogeneousPole_[axis::y] * _nReciprocal;
    nWeight_ = ptHomogeneousPole_[axis::z];
}

template<>
void CNurbsRefiner<DPOINT3, DPOINT4>::UpgradePoles(const DPOINT3& ptPole_, double nWeight_, DPOINT4& ptHomogeneousPole_) const
{
    NEAT_RAISE_IF(DOUBLE_EQU_ZERO(nWeight_), g_strNurbsParamsError);
    ptHomogeneousPole_[axis::x] = ptPole_[axis::x] * nWeight_;
    ptHomogeneousPole_[axis::y] = ptPole_[axis::y] * nWeight_;
    ptHomogeneousPole_[axis::z] = ptPole_[axis::z] * nWeight_;
    ptHomogeneousPole_[3] = nWeight_;
}

template<>
void CNurbsRefiner<DPOINT3, DPOINT4>::DowngradePoles(const DPOINT4& ptHomogeneousPole_, DPOINT3& ptPole_, double& nWeight_) const
{
    NEAT_RAISE_IF(DOUBLE_EQU_ZERO(ptHomogeneousPole_[3]), g_strNurbsParamsError);
    double _nReciprocal = 1.0 / ptHomogeneousPole_[3];
    ptPole_[axis::x] = ptHomogeneousPole_[axis::x] * _nReciprocal;
    ptPole_[axis::y] = ptHomogeneousPole_[axis::y] * _nReciprocal;
    ptPole_[axis::z] = ptHomogeneousPole_[axis::z] * _nReciprocal;
    nWeight_ = ptHomogeneousPole_[3];
}