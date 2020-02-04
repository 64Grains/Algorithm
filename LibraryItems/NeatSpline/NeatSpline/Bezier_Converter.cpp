/***************************************************************************************************
 Created on: 2020-02-04
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
// class CBezierConverter2D
CBezierConverter2D::CBezierConverter2D()
{
}

CBezierConverter2D::~CBezierConverter2D()
{
}

void CBezierConverter2D::ConvertBezierToPolyline(const BezierNode2D* pBezierNode_, double nDeflection_, Polyline2D& Polyline2D_)
{
    NEAT_RAISE(pBezierNode_ == nullptr, g_strEmptyPointer);

    // If the Bezier node is one-degree, output directly
    if (pBezierNode_->vecPoles.size() == 2) {
        OutputSegment(pBezierNode_->vecPoles.front(), pBezierNode_->vecPoles.back(), Polyline2D_);
        return;
    }

    // Scatter bezier node into scattered points for checking the result of biarc fit
    m_nDeflection = nDeflection_;
    m_vecScatterNodes.clear();
    ScatterBezier(pBezierNode_, nDeflection_ * 0.5, m_vecScatterNodes);

    // Check the scatter result : if it can be approximated with a straight line, output directly
    if (m_vecScatterNodes.size() < 3) {
        NEAT_RAISE_IF(m_vecScatterNodes.empty(), g_strBezierParamsError);
        OutputSegment(m_vecScatterNodes.front().ptPoint, m_vecScatterNodes.back().ptPoint, Polyline2D_);
        return;
    }

    // Check if the scatter bezier node is reversed and divide at the reverse
    VECDOUBLE _vecReverseKnots;
    SearchReverseKnots(_vecReverseKnots);

    // Knot interval
    struct KnotInterval
    {
        double nStartKnot = 0.0;
        double nEndKnot = 0.0;
    };

    // Collect knot intervals and convert each bezier node segment
    std::list<KnotInterval> _listKnotIntervals;
    double _nStartKnot = pBezierNode_->nStartKnot;
    for (const auto& _nDivideKnot : _vecReverseKnots) {
        _listKnotIntervals.emplace_back(KnotInterval{ _nStartKnot, _nDivideKnot });
        _nStartKnot = _nDivideKnot;
    }

    _listKnotIntervals.emplace_back(KnotInterval{ _nStartKnot, pBezierNode_->nEndKnot });

    while (!_listKnotIntervals.empty()) {
        double _nDivideKnot = 0.0;
        const KnotInterval& _KnotInterval = _listKnotIntervals.front();
        if (ConvertBezierToPolyline(_KnotInterval.nStartKnot, _KnotInterval.nEndKnot, _nDivideKnot, Polyline2D_)) {
            _listKnotIntervals.pop_front();
        }
        else {
            double _nOrigStartKnot = _KnotInterval.nStartKnot;
            double _nOrigEndKnot = _KnotInterval.nEndKnot;
            _listKnotIntervals.pop_front();
            _listKnotIntervals.emplace_front(KnotInterval{ _nDivideKnot, _nOrigEndKnot });
            _listKnotIntervals.emplace_front(KnotInterval{ _nOrigStartKnot, _nDivideKnot });
        }
    }
}

void CBezierConverter2D::OutputSegment(const DPOINT2& ptStart_, const DPOINT2& ptEnd_, Polyline2D& Polyline2D_)
{
    if (Polyline2D_.vecNodes.empty()) {
        Polyline2D_.nStartX = ptStart_[axis::x];
        Polyline2D_.nStartY = ptStart_[axis::y];
    }

    Polyline2D_.vecNodes.emplace_back(Polyline2D::Node2D{ ptEnd_[axis::x], ptEnd_[axis::y], 0.0 });
}

void CBezierConverter2D::SearchReverseKnots(VECDOUBLE& vecReverseKnots_) const
{
    if (m_vecScatterNodes.size() < 3) {
        return;
    }

    // Determine whether the bezier node is reversed  by the number of changes in the outer product sign,
    // and record the knots at the reverse
    int _nDirChangeCount = 0;
    double _nBaseCrossProduct = CrossProduct(m_vecScatterNodes[0].ptDeriv1, m_vecScatterNodes[1].ptDeriv1);
    for (size_t i = 2, _nSize = m_vecScatterNodes.size() - 1; i < _nSize; ++i) {
        double _nCrossProduct = CrossProduct(m_vecScatterNodes[i - 1].ptDeriv1, m_vecScatterNodes[i].ptDeriv1);
        if (_nBaseCrossProduct * _nCrossProduct < 0.0) {
            if (++_nDirChangeCount > 1) {
                vecReverseKnots_.push_back(m_vecScatterNodes[i - 1].nKnot);
                _nDirChangeCount = 0;
            }

            _nBaseCrossProduct = _nCrossProduct;
        }
    }
}

bool CBezierConverter2D::ConvertBezierToPolyline(double nStartKnot_, double nEndKnot_, double& nDivideKnot_, Polyline2D& Polyline2D_)
{
    // Find point coordinates and first-order derivatives corresponding to knots
    size_t _nStartIndex = FindSpanForKnot(nStartKnot_);
    size_t _nEndIndex = FindSpanForKnot(nEndKnot_);
    DPOINT2 _ptStartPoint = m_vecScatterNodes[_nStartIndex].ptPoint;
    DPOINT2 _ptStartTangnet = m_vecScatterNodes[_nStartIndex].ptDeriv1;
    DPOINT2 _ptEndPoint = m_vecScatterNodes[_nEndIndex].ptPoint;
    DPOINT2 _ptEndTangnet = m_vecScatterNodes[_nEndIndex].ptDeriv1;

    // Biarc fit
    Polyline2D _Polyline2D;
    BiarcFit(_ptStartPoint, _ptStartTangnet, _ptEndPoint, _ptEndTangnet, _Polyline2D);
    NEAT_RAISE_IF(_Polyline2D.vecNodes.size() != 2, g_strBiarcFitParamsError);
    if (_Polyline2D.vecNodes.size() != 2) {
        // Line segment length is zero
        return true;
    }

    // Check fitting error
    DPOINT2 _ptConnectPoint(_Polyline2D.vecNodes.front().nEndX, _Polyline2D.vecNodes.front().nEndY);
    bool _bLastIsLine = DOUBLE_EQU_ZERO(_Polyline2D.vecNodes.front().nBulge);
    bool _bNextIsLine = DOUBLE_EQU_ZERO(_Polyline2D.vecNodes.back().nBulge);
    ArcNode2D _ArcLast, _ArcNext;
    if (!_bLastIsLine) {
        RetrieveArcNode2D(_ptStartPoint, _ptConnectPoint, _Polyline2D.vecNodes.front().nBulge, _ArcLast);
    }

    if (!_bNextIsLine) {
        RetrieveArcNode2D(_ptConnectPoint, _ptEndPoint, _Polyline2D.vecNodes.back().nBulge, _ArcNext);
    }

    double _nMaxError = 0.0;
    for (size_t i = _nStartIndex + 1; i < _nEndIndex; ++i) {
        const DPOINT2& _ptAimPoint = m_vecScatterNodes[i].ptPoint;
        double _nMaxError1 = _bLastIsLine
            ? DistPointToSegment(_ptStartPoint, _ptConnectPoint, _ptAimPoint)
            : DistPointToArc(_ptStartPoint, _ArcLast, _ptAimPoint);
        if (Precision::IsInvalidDouble(_nMaxError1)) {
            _nMaxError1 = min((_ptAimPoint - _ptStartPoint).GetLength(), (_ptAimPoint - _ptConnectPoint).GetLength());
        }

        double _nMaxError2 = _bNextIsLine
            ? DistPointToSegment(_ptConnectPoint, _ptEndPoint, _ptAimPoint)
            : DistPointToArc(_ptConnectPoint, _ArcNext, _ptAimPoint);
        if (Precision::IsInvalidDouble(_nMaxError2)) {
            _nMaxError2 = min((_ptAimPoint - _ptConnectPoint).GetLength(), (_ptAimPoint - _ptEndPoint).GetLength());
        }

        double _nMinError = min(_nMaxError1, _nMaxError2);
        if (_nMinError > _nMaxError) {
            _nMaxError = _nMinError;
            nDivideKnot_ = m_vecScatterNodes[i].nKnot;
        }
    }

    if (!DOUBLE_EQU(_nMaxError, m_nDeflection) && (_nMaxError > m_nDeflection)) {
        return false;
    }

    if (Polyline2D_.vecNodes.empty()) {
        Polyline2D_.nStartX = _Polyline2D.nStartX;
        Polyline2D_.nStartY = _Polyline2D.nStartY;
    }

    for (auto& _node : _Polyline2D.vecNodes) {
        Polyline2D_.vecNodes.emplace_back(_node);
    }

    return true;
}

size_t CBezierConverter2D::FindSpanForKnot(double nKnot_) const
{
    if (DOUBLE_EQU(nKnot_, m_vecScatterNodes.front().nKnot)) {
        return 0;
    }
    else if (DOUBLE_EQU(nKnot_, m_vecScatterNodes.back().nKnot)) {
        return m_vecScatterNodes.size() - 1;
    }

    // Dichotomous lookup: Conventional knot value must be found in the container
    size_t _nLow = 0;
    size_t _nHigh = m_vecScatterNodes.size() - 1;
    size_t _nMid = (_nLow & _nHigh) + ((_nLow ^ _nHigh) >> 1);
    while (true) {
        if (DOUBLE_EQU(nKnot_, m_vecScatterNodes[_nMid].nKnot)) {
            return _nMid;
        }
        else if (nKnot_ < m_vecScatterNodes[_nMid].nKnot) {
            _nHigh = _nMid;
        }
        else {
            _nLow = _nMid + 1;
        }

        NEAT_RAISE_IF(_nHigh - _nLow < 1, g_strBezierParamsError);
        _nMid = (_nLow & _nHigh) + ((_nLow ^ _nHigh) >> 1);
    }
}

void CBezierConverter2D::BiarcFit(const DPOINT2& ptStartPoint_, const DPOINT2& ptStartTangent_, const DPOINT2& ptEndPoint_,
    const DPOINT2& ptEndTangent_, Polyline2D& Polyline2D_) const
{
    NEAT_RAISE(DOUBLE_EQU_ZERO(ptStartTangent_.GetLength()), g_strBiarcFitParamsError);
    NEAT_RAISE(DOUBLE_EQU_ZERO(ptEndTangent_.GetLength()), g_strBiarcFitParamsError);

    // Set start point
    Polyline2D_.nStartX = ptStartPoint_[axis::x];
    Polyline2D_.nStartY = ptStartPoint_[axis::y];

    // Normalized tangent vector
    DPOINT2 _ptStartTangent = ptStartTangent_ / ptStartTangent_.GetLength();
    DPOINT2 _ptEndTangnet = ptEndTangent_ / ptEndTangent_.GetLength();

    DPOINT2 _ptStartEnd = ptEndPoint_ - ptStartPoint_;
    double _nLength = _ptStartEnd.GetLength();
    if (DOUBLE_EQU_ZERO(_nLength)) {
        Polyline2D_.vecNodes.emplace_back(Polyline2D::Node2D{ ptEndPoint_[axis::x], ptEndPoint_[axis::y], 0.0 });
        return;
    }

    // Suppose the connection point of two arcs is Pm
    // For the first arc, the intersection of the tangent passing through the start point and the tangent passing through the connection point is q1,
    // and the distance from q1 to the start point is d1.
    // For the second arc, the intersection of the tangent passing through the end point and the tangent passing through the connection point is q2,
    // and the distance from q2 to the end point is d2.
    // To simplify the calculation and make the fitted arcs relatively balanced, let d1 = d2, and calculate d1 and Pm in this order.
    double _nDist = 0.0;
    double _nSquareLength = _ptStartEnd.GetSquareLength();

    // If the denominator of the quadratic equation root is zero, special treatment is required
    double _nDenominator = 2.0 * (1.0 - DotProduct(_ptStartTangent, _ptEndTangnet));
    if (DOUBLE_EQU_ZERO(_nDenominator)) {
        // Start and end tangent vectors are the same
        _nDenominator = 4.0 * DotProduct(_ptStartEnd, _ptEndTangnet);

        // In this special case, if the denominator is zero, the fit result can only be two semicircles
        if (DOUBLE_EQU_ZERO(_nDenominator)) {
            // The line between start and end points is perpendicular to the tangent vector
            DPOINT2 _ptConnect = (ptStartPoint_ + ptEndPoint_) * 0.5;

            // The direction of the second arc
            double _nSecondDir = Sign(CrossProduct(_ptStartEnd, _ptEndTangnet));

            Polyline2D_.vecNodes.emplace_back(Polyline2D::Node2D{ _ptConnect[axis::x], _ptConnect[axis::y], -1.0 * _nSecondDir });
            Polyline2D_.vecNodes.emplace_back(Polyline2D::Node2D{ ptEndPoint_[axis::x], ptEndPoint_[axis::y], _nSecondDir });
            return;
        }
        else {
            _nDist = _nSquareLength / _nDenominator;
        }
    }
    else {
        // Use positive root of the quadratic equation
        double _nDotTangent = DotProduct(_ptStartEnd, _ptStartTangent + _ptEndTangnet);
        double _nDiscriminant = _nDotTangent * _nDotTangent + _nDenominator * _nSquareLength;
        _nDist = (-1.0 * _nDotTangent + sqrt(fabs(_nDiscriminant))) / _nDenominator;
    }

    // Calculate connection point Pm
    DPOINT2 _ptConnect = 0.5 * (ptStartPoint_ + ptEndPoint_ + _nDist * (_ptStartTangent - _ptEndTangnet));

    // Calculate Fitting Arc
    struct {
        DPOINT2 ptCenter;
        double nRadius = 0.0;
        double nCentralAngle = 0.0;
    } _arcLast, _arcNext;

    ComputeArc(ptStartPoint_, _ptStartTangent, _ptConnect, _arcLast.ptCenter, _arcLast.nRadius, _arcLast.nCentralAngle);
    ComputeArc(ptEndPoint_, -1.0 * _ptEndTangnet, _ptConnect, _arcNext.ptCenter, _arcNext.nRadius, _arcNext.nCentralAngle);

    double _nFirstBulge = Precision::IsInfinityDouble(_arcLast.nRadius) ? 0.0 : tan(0.25 * _arcLast.nCentralAngle);
    Polyline2D_.vecNodes.emplace_back(Polyline2D::Node2D{ _ptConnect[axis::x], _ptConnect[axis::y], _nFirstBulge });

    double _nSecondBulge = Precision::IsInfinityDouble(_arcNext.nRadius) ? 0.0 : tan(-0.25 * _arcNext.nCentralAngle);
    Polyline2D_.vecNodes.emplace_back(Polyline2D::Node2D{ ptEndPoint_[axis::x], ptEndPoint_[axis::y], _nSecondBulge });
}

void CBezierConverter2D::ComputeArc(const DPOINT2& ptStartPoint_, const DPOINT2& ptStartTangent_, const DPOINT2& ptEndPoint_,
    DPOINT2& ptCenter_, double& nRadius_, double nCentralAngle_) const
{
    NEAT_RAISE_IF(!DOUBLE_EQU(ptStartTangent_.GetLength(), 1.0), g_strBiarcFitParamsError);

    // Vector from start to end
    DPOINT2 _ptStartEnd = ptEndPoint_ - ptStartPoint_;

    // Vector perpendicular to the start tangent
    DPOINT2 _ptPerpVector(-1.0 * ptStartTangent_[axis::y], ptStartTangent_[axis::x]);

    const double _nDenominator = 2.0 * DotProduct(_ptStartEnd, _ptPerpVector);
    if (DOUBLE_EQU_ZERO(_nDenominator)) {
        // Arc radius is infinite, use straight line instead
        nRadius_ = Precision::InfinityDouble();
        return;
    }

    // Calculate the center and radius of arc
    double _nRadius = _ptStartEnd.GetSquareLength() / _nDenominator;
    ptCenter_ = ptStartPoint_ + _nRadius * _ptPerpVector;
    nRadius_ = fabs(_nRadius);

    // Calculate the central angle of arc
    double _nCentralAngle = 0.0;
    if (!DOUBLE_EQU_ZERO(nRadius_)) {
        DPOINT2 _ptCenterStart = ptStartPoint_ - ptCenter_;
        DPOINT2 _ptCenterEnd = ptEndPoint_ - ptCenter_;
        double _nCosAngle = DotProduct(_ptCenterStart, _ptCenterEnd) / (nRadius_ * nRadius_);
        __CUT(_nCosAngle, -1.0, 1.0);

        _nCentralAngle = acos(_nCosAngle);

        // Judge whether if the arc is a minor arc
        bool _bMinorArc = DotProduct(_ptStartEnd, ptStartTangent_) >= 0.0;
        if (!_bMinorArc) {
            _nCentralAngle = 2.0 * Precision::PIE() - _nCentralAngle;
        }

        // Judging the arc direction: counterclockwise represents positive, clockwise represents negative
        double _nDir = Sign(CrossProduct(_ptCenterStart, ptStartTangent_));
        _nCentralAngle *= _nDir;
    }

    nCentralAngle_ = _nCentralAngle;
}

double CBezierConverter2D::DistPointToSegment(const DPOINT2& ptStartPoint_, const DPOINT2& ptEndPoint_, const DPOINT2& ptAimPoint_) const
{
    // First determine whether the point-to-line foot is in the line segment
    DPOINT2 _ptStartAim = ptAimPoint_ - ptStartPoint_;
    DPOINT2 _ptStartEnd = ptEndPoint_ - ptStartPoint_;
    DPOINT2 _ptAimEnd = ptEndPoint_ - ptAimPoint_;
    if (DotProduct(_ptStartAim, _ptStartEnd) < 0.0 || DotProduct(_ptStartEnd, _ptAimEnd) < 0.0) {
        // Point outside line segment
        return Precision::InvalidDouble();
    }
    else {
        // Point in line segment
        return fabs(CrossProduct(_ptStartAim, _ptStartEnd) / _ptStartEnd.GetLength());
    }
}

double CBezierConverter2D::DistPointToArc(const DPOINT2& ptStartPoint_, const ArcNode2D& ArcNode_, const DPOINT2& ptAimPoint_) const
{
    // First determine whether the point is in the sector area where the arc is located
    DPOINT2 _ptCenterStart = ptStartPoint_ - ArcNode_.ptCenter;
    DPOINT2 _ptCenterAim = ptAimPoint_ - ArcNode_.ptCenter;
    double _nSquareRadius = ArcNode_.nRadius * ArcNode_.nRadius;
    double _nSign = Sign(ArcNode_.nEndAngle - ArcNode_.nStartAngle);
    double _nRotateAngle = ComputeAngle(DotProduct(_ptCenterStart, _ptCenterAim) / _nSquareRadius,
        _nSign * CrossProduct(_ptCenterStart, _ptCenterAim) / _nSquareRadius);

    if (fabs(ArcNode_.nEndAngle - ArcNode_.nStartAngle) > _nRotateAngle) {
        // Point in the sector area
        return fabs(_ptCenterAim.GetLength() - ArcNode_.nRadius);
    }
    else {
        // Point outside the sector area
        return Precision::InvalidDouble();
    }
}