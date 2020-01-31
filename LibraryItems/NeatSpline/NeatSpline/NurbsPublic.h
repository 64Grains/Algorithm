/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NURBS_PUBLIC_H_20200130__
#define __NURBS_PUBLIC_H_20200130__

/**
 * This file defines public functions for Non-uniform rational B-spline curve.
 */
namespace neat
{
    /**
     * Macro definition
     */
#ifdef DOUBLE_EQU
    #undef DOUBLE_EQU
#endif

#ifdef DOUBLE_EQU_ZERO
    #undef DOUBLE_EQU_ZERO
#endif

#define DOUBLE_EQU(x, y)    Precision::IsAlmostEqual(x, y, Precision::RealTolerance())
#define DOUBLE_EQU_ZERO(x)  DOUBLE_EQU(x, 0.0)

    /**
     * Type definition
     */
    typedef CAxisPoint<double, 4> DPOINT4;

    /**
     * Check if nurbs node is rational
     */
    template<typename TYPE>
    bool CheckNurbsNodeRational(const NurbsNode<TYPE>* pNurbsNode_)
    {
        NEAT_RAISE(pNurbsNode_ == nullptr, g_strEmptyPointer);

        // Check if nurbs node is legal
        NEAT_RAISE(pNurbsNode_->nDegree < 1 || pNurbsNode_->vecKnots.empty() || pNurbsNode_->vecPoles.empty(), g_strNurbsParamsError);
        NEAT_RAISE(pNurbsNode_->vecKnots.size() != pNurbsNode_->vecPoles.size() + pNurbsNode_->nDegree + 1, g_strNurbsParamsError);
        NEAT_RAISE(!pNurbsNode_->vecWeights.empty() && pNurbsNode_->vecWeights.size() != pNurbsNode_->vecPoles.size(), g_strNurbsParamsError);

        // Check if nurbs node is rational
        for (size_t i = 1, _nSize = pNurbsNode_->vecWeights.size(); i < _nSize; ++i) {
            if (!DOUBLE_EQU(pNurbsNode_->vecWeights[0], pNurbsNode_->vecWeights[i])) {
                return true;
            }
        }

        return false;
    }

    /**
     * Convert Bezier node to Nurbs node
     */
    template<typename TYPE>
    void ConvertBezierToNurbs(const BezierNode<TYPE>* pBezierNode_, NurbsNode<TYPE>* pNurbsNode_)
    {
        NEAT_RAISE(pBezierNode_ == nullptr || pNurbsNode_ == nullptr, g_strEmptyPointer);

        // Check if bezier node is legal
        NEAT_RAISE(pBezierNode_->vecPoles.size() < 2 || pBezierNode_->nStartKnot >= pBezierNode_->nEndKnot, g_strBezierParamsError);
        NEAT_RAISE(!pBezierNode_->vecWeights.empty() && pBezierNode_->vecWeights.size() != pBezierNode_->vecPoles.size(), g_strBezierParamsError);

        // Convert Bezier node
        pNurbsNode_->nDegree = static_cast<int>(pBezierNode_->vecPoles.size()) - 1;
        pNurbsNode_->vecKnots.clear();
        for (int i = 0; i <= pNurbsNode_->nDegree; ++i) {
            pNurbsNode_->vecKnots.push_back(pBezierNode_->nStartKnot);
        }

        for (int i = 0; i <= pNurbsNode_->nDegree; ++i) {
            pNurbsNode_->vecKnots.push_back(pBezierNode_->nEndKnot);
        }

        pNurbsNode_->vecPoles = pBezierNode_->vecPoles;
        pNurbsNode_->vecWeights = pBezierNode_->vecWeights;
    }

    // Return the span i of nKnot_ (U_i <= nKnot_ < U_i+1)
    int FindSpanForKnot(const VECDOUBLE& vecKnots_, double nKnot_, int nStartIndex_, int nEndIndex_, int& nCacheIndex_);

} // End namespace neat

#endif // __NURBS_PUBLIC_H_20200130__