/***************************************************************************************************
 Created on: 2020-02-04
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __BEZIER_CONVERTER_H_20200204__
#define __BEZIER_CONVERTER_H_20200204__

/**
 * This file defines the converter for bezier node.
 */
namespace neat
{
    /**
     * Convert two-dimensional bezier node to polyline
     */
    class CBezierConverter2D final
    {
    public:
        CBezierConverter2D();
        ~CBezierConverter2D();

        void ConvertBezierToPolyline(const BezierNode2D* pBezierNode_, double nDeflection_, Polyline2D& Polyline2D_);

    private:
        CBezierConverter2D(const CBezierConverter2D&) = delete;
        CBezierConverter2D& operator=(const CBezierConverter2D&) = delete;

        /**
         * Output line segment to polyline
         */
        void OutputSegment(const DPOINT2& ptStart_, const DPOINT2& ptEnd_, Polyline2D& Polyline2D_);

        /**
         * Search reverse knots
         */
        void SearchReverseKnots(VECDOUBLE& vecReverseKnots_) const;

        /**
         * Convert bezier node at specified interval to polyline
         * @ Return true if the conversion was successful, and the result of the conversion is stored in the polyline
         * @ Return false if the conversion was failed, and the knot value corresponding to the maximum error is output.
         *   The bezier node is split at this knot before the next iteration.
         */
        bool ConvertBezierToPolyline(double nStartKnot_, double nEndKnot_, double& nDivideKnot_, Polyline2D& Polyline2D_);

        /**
         * Find the index value corresponding to the knot
         */
        size_t FindSpanForKnot(double nKnot_) const;

        /**
         * Biarc fit : Calculate fitted arc information based on start point, start tangent, end point, and end tangent.
         */
        void BiarcFit(const DPOINT2& ptStartPoint_, const DPOINT2& ptStartTangent_, const DPOINT2& ptEndPoint_,
            const DPOINT2& ptEndTangent_, Polyline2D& Polyline2D_) const;

        /**
         * Calculate arc / line information based on start point, start tangent, and end point
         * @ if the radius is infinity, it represents a straight line
         */
        void ComputeArc(const DPOINT2& ptStartPoint_, const DPOINT2& ptStartTangent_, const DPOINT2& ptEndPoint_,
            DPOINT2& ptCenter_, double& nRadius_, double nCentralAngle_) const;

        /**
         * Distance from point to line segment
         */
        double DistPointToSegment(const DPOINT2& ptStartPoint_, const DPOINT2& ptEndPoint_, const DPOINT2& ptAimPoint_) const;

        /**
         * Distance from point to arc
         */
        double DistPointToArc(const DPOINT2& ptStartPoint_, const ArcNode2D& ArcNode_, const DPOINT2& ptAimPoint_) const;

    private:
        std::vector<ScatterNode2D> m_vecScatterNodes;
        double m_nDeflection = 0.0;
    };

} // End namespace neat

#endif // __BEZIER_CONVERTER_H_20200204__