/***************************************************************************************************
 Created on: 2020-01-31
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_FITTER_H_20200131__
#define __NEAT_FITTER_H_20200131__

/**
 * This file defines the interface for fitting splines.
 */
namespace neat
{
    class CNurbsInterpPointer;

    /**
     * The interp for one-dimensional nurbs node
     */
    class NEATSPLINE_API CNurbsInterp1D final
    {
    public:
        CNurbsInterp1D();
        ~CNurbsInterp1D();

        // Set start tangent vector
        void SetStartTangent(double nStartTangent_);

        // Set end tangent vector
        void SetEndTangent(double nEndTangent_);

        // Set knot vector
        void SetKnotVector(const VECDOUBLE& vecKnots_);

        // NURBS curve interpolation
        bool InterpNurbsCurve(const VECDOUBLE& vecFitPoints_, NurbsNode1D& NurbsNode_);

    private:
        CNurbsInterp1D(const CNurbsInterp1D&) = delete;
        CNurbsInterp1D& operator=(const CNurbsInterp1D&) = delete;

        CNurbsInterpPointer* m_pInterpPointer;
    };

    /**
     * The interp for two-dimensional nurbs node
     */
    class NEATSPLINE_API CNurbsInterp2D final
    {
    public:
        CNurbsInterp2D();
        ~CNurbsInterp2D();

        // Set start tangent vector
        void SetStartTangent(const DPOINT2& nxStartTangent_);

        // Set end tangent vector
        void SetEndTangent(const DPOINT2& nxEndTangent_);

        // Set knot vector
        void SetKnotVector(const VECDOUBLE& vecKnots_);

        // NURBS curve interpolation
        bool InterpNurbsCurve(const VECDPOINT2& vecFitPoints_, NurbsNode2D& NurbsNode_);

    private:
        CNurbsInterp2D(const CNurbsInterp2D&) = delete;
        CNurbsInterp2D& operator=(const CNurbsInterp2D&) = delete;

        CNurbsInterpPointer* m_pInterpPointer;
    };

    /**
     * The interp for three-dimensional nurbs node
     */
    class NEATSPLINE_API CNurbsInterp3D final
    {
    public:
        CNurbsInterp3D();
        ~CNurbsInterp3D();

        // Set start tangent vector
        void SetStartTangent(const DPOINT3& nxStartTangent_);

        // Set end tangent vector
        void SetEndTangent(const DPOINT3& nxEndTangent_);

        // Set knot vector
        void SetKnotVector(const VECDOUBLE& vecKnots_);

        // NURBS curve interpolation
        bool InterpNurbsCurve(const VECDPOINT3& vecFitPoints_, NurbsNode3D& NurbsNode_);

    private:
        CNurbsInterp3D(const CNurbsInterp3D&) = delete;
        CNurbsInterp3D& operator=(const CNurbsInterp3D&) = delete;

        CNurbsInterpPointer* m_pInterpPointer;
    };

} // End namespace neat

#endif // __NEAT_FITTER_H_20200131__