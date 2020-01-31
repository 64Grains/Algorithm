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
// global function
int neat::FindSpanForKnot(const VECDOUBLE& vecKnots_, double nKnot_, int nStartIndex_, int nEndIndex_, int& nCacheIndex_)
{
    NEAT_RAISE_IF(nKnot_ < vecKnots_[nStartIndex_] || nKnot_ > vecKnots_[nEndIndex_], g_strNurbsParamsError);

    // matched in cache
    if (nKnot_ >= vecKnots_[nCacheIndex_] && nKnot_ < vecKnots_[nCacheIndex_ + 1]) {
        return nCacheIndex_;
    }

    // the last knot
    if (DOUBLE_EQU(nKnot_, vecKnots_[nEndIndex_])) {
        int _nKnotIndex = nEndIndex_ - 1;
        while (DOUBLE_EQU(vecKnots_[_nKnotIndex], vecKnots_[nEndIndex_])) {
            --_nKnotIndex;
        }

        nCacheIndex_ = _nKnotIndex;
        return nCacheIndex_;
    }

    // search by dichotomy
    int _nLow = nStartIndex_;
    int _nHigh = nEndIndex_;
    int _nMid = (_nLow & _nHigh) + ((_nLow ^ _nHigh) >> 1);
    while (true) {
        if (nKnot_ < vecKnots_[_nMid]) {
            _nHigh = _nMid;
        }
        else if (nKnot_ >= vecKnots_[_nMid + 1]) {
            _nLow = _nMid;
        }
        else {
            break;
        }

        _nMid = (_nLow & _nHigh) + ((_nLow ^ _nHigh) >> 1);
    }

    nCacheIndex_ = _nMid;
    return nCacheIndex_;
}