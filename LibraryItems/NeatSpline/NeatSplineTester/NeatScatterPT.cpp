/***************************************************************************************************
 Created on: 2020-02-01
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library tester application.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#include "NeatSplineTester.h"
#include <fstream>
#include <sstream>

using namespace neat;
using namespace tester;

/**
 * Read the scatter points corresponding to the nurbs spline under high accuracy from file.
 */
static void ReadScatterResult(const std::string& strFilePath_, VECDPOINT3& vecScatterResult_);
static void CheckPolylinesSimilar(const VECDPOINT2& vecLeft_, const VECDPOINT2& vecRight_, double nTolerance_);
static void CheckPolylinesSimilar(const Polyline2D& Polyline_, const VECDPOINT2& vecPoints_, double nTolerance_);
static void CheckPolylinesSimilar(const VECDPOINT3& vecLeft_, const VECDPOINT3& vecRight_, double nTolerance_);
static void CheckPolylinesSimilar(const std::vector<ScatterNode2D>& vecNodes_, const VECDPOINT2& vecPoints_, double nTolerance_);
static void CheckPolylinesSimilar(const std::vector<ScatterNode3D>& vecNodes_, const VECDPOINT3& vecPoints_, double nTolerance_);
static double DistPointToSegment(const DPOINT2& ptStartPoint_, const DPOINT2& ptEndPoint_, const DPOINT2& ptAimPoint_);
static double DistPointToSegment(const DPOINT3& ptStartPoint_, const DPOINT3& ptEndPoint_, const DPOINT3& ptAimPoint_);

/**
 * This file parameter tests the interface for scatting splines.
 */
const static CTestFileInfoList gs_listTestFileInfo;

class CNurbsScatterTest : public ::testing::TestWithParam<TestFileInfo>
{
};

TEST_P(CNurbsScatterTest, NurbsFiles)
{
    const TestFileInfo& _fileInfo = GetParam();

    // Read verification result
    VECDPOINT3 _vecScatterResult;
    ReadScatterResult(_fileInfo.strFileDir + _fileInfo.strScatterFile, _vecScatterResult);

    // Read test file
    std::vector<NurbsNode3D> _vecNurbsNodes;
    ReadNurbsFromFile(_fileInfo.strFileDir + _fileInfo.strFileName, _vecNurbsNodes);

    double _szDeflection[] = { 0.001, 0.01 };
    double _nCheckErrorRatio = 2.0;

    // Check if it can downgrade
    VECDPOINT2 _vecScatterResult2D;
    std::vector<NurbsNode2D> _vecNurbsNodes2D;
    if (DowngradeDPoint(_vecScatterResult, _vecScatterResult2D) && DowngradeNurbsNode(_vecNurbsNodes, _vecNurbsNodes2D)) {
        std::vector<ScatterNode2D> _vecScatterNodes;
        VECDPOINT2 _vecScatterPoints;
        Polyline2D _Polyline2D;
        bool _bOneDegree = false;
        for (const auto& _NurbsNode : _vecNurbsNodes2D) {
            std::vector<ScatterNode2D> _vecNodesTemp;
            VECDPOINT2 _vecPointsTemp;
            Polyline2D _PolylineTemp;
            BezierNode2D _BezierNode;
            if (!_bOneDegree && _NurbsNode.nDegree == 1) {
                _bOneDegree = true;
            }

            if (TransformNurbsNode(_NurbsNode, _BezierNode)) {
                if (_NurbsNode.nDegree > 1) {
                    ScatterBezier(&_BezierNode, _szDeflection[0], _vecNodesTemp);
                }

                ScatterBezier(&_BezierNode, _szDeflection[0], _vecPointsTemp);
                ScatterBezier(&_BezierNode, _szDeflection[1], _PolylineTemp);
            }
            else {
                if (_NurbsNode.nDegree > 1) {
                    ScatterNurbs(&_NurbsNode, _szDeflection[0], _vecNodesTemp);
                }

                ScatterNurbs(&_NurbsNode, _szDeflection[0], _vecPointsTemp);
                ScatterNurbs(&_NurbsNode, _szDeflection[1], _PolylineTemp);
            }

            for (const auto& _ScatterNode : _vecNodesTemp) {
                _vecScatterNodes.emplace_back(_ScatterNode);
            }

            for (const auto& _ptPoint : _vecPointsTemp) {
                _vecScatterPoints.emplace_back(_ptPoint);
            }

            if (_Polyline2D.vecNodes.empty()) {
                _Polyline2D.nStartX = _PolylineTemp.nStartX;
                _Polyline2D.nStartY = _PolylineTemp.nStartY;
            }
            else {
                _Polyline2D.vecNodes.emplace_back(Polyline2D::Node2D{ _PolylineTemp.nStartX, _PolylineTemp.nStartY, 0.0 });
            }

            for (const auto& _node : _PolylineTemp.vecNodes) {
                _Polyline2D.vecNodes.emplace_back(_node);
            }
        }

        if (!_bOneDegree) {
            CheckPolylinesSimilar(_vecScatterNodes, _vecScatterResult2D, _szDeflection[0] * _nCheckErrorRatio);
        }

        CheckPolylinesSimilar(_vecScatterPoints, _vecScatterResult2D, _szDeflection[0] * _nCheckErrorRatio);
        CheckPolylinesSimilar(_Polyline2D, _vecScatterResult2D, _szDeflection[1] * _nCheckErrorRatio);
    }
    else {
        std::vector<ScatterNode3D> _vecScatterNodes;
        VECDPOINT3 _vecScatterPoints;
        for (const auto& _NurbsNode : _vecNurbsNodes) {
            std::vector<ScatterNode3D> _vecNodesTemp;
            VECDPOINT3 _vecPointsTemp;
            BezierNode3D _BezierNode;
            if (TransformNurbsNode(_NurbsNode, _BezierNode)) {
                ScatterBezier(&_BezierNode, _szDeflection[0], _vecNodesTemp);
                ScatterBezier(&_BezierNode, _szDeflection[0], _vecPointsTemp);
            }
            else {
                ScatterNurbs(&_NurbsNode, _szDeflection[0], _vecNodesTemp);
                ScatterNurbs(&_NurbsNode, _szDeflection[0], _vecPointsTemp);
            }

            for (const auto& _ScatterNode : _vecNodesTemp) {
                _vecScatterNodes.emplace_back(_ScatterNode);
            }

            for (const auto& _ptPoint : _vecPointsTemp) {
                _vecScatterPoints.emplace_back(_ptPoint);
            }
        }

        CheckPolylinesSimilar(_vecScatterNodes, _vecScatterResult, _szDeflection[0] * _nCheckErrorRatio);
        CheckPolylinesSimilar(_vecScatterPoints, _vecScatterResult, _szDeflection[0] * _nCheckErrorRatio);
    }
}

INSTANTIATE_TEST_CASE_P(NeatScatterPT, CNurbsScatterTest, testing::ValuesIn(gs_listTestFileInfo));

//////////////////////////////////////////////////////////////////////////
// local function
static void ReadScatterResult(const std::string& strFilePath_, VECDPOINT3& vecScatterResult_)
{
    if (strFilePath_.empty()) {
        printf("Invalid file path!\n");
        return;
    }

    std::ifstream _infile;
    _infile.open(strFilePath_.c_str(), std::fstream::in);
    if (!_infile) {
        printf("Open file (%s) failed!\n", strFilePath_.c_str());
        return;
    }

    vecScatterResult_.clear();

    std::string _strBuffer;
    while (getline(_infile, _strBuffer)) {
        DPOINT3 _ptPoint;
        std::istringstream _instring(_strBuffer);
        if (_instring >> _ptPoint[axis::x] >> _ptPoint[axis::y] >> _ptPoint[axis::z]) {
            vecScatterResult_.emplace_back(_ptPoint);
        }
    }
}

static void CheckPolylinesSimilar(const VECDPOINT2& vecLeft_, const VECDPOINT2& vecRight_, double nTolerance_)
{
    // Check start and end coordinates
    for (int i = axis::x; i <= axis::y; ++i) {
        ASSERT_NEAR(vecLeft_.front()[i], vecRight_.front()[i], nTolerance_);
        ASSERT_NEAR(vecLeft_.back()[i], vecRight_.back()[i], nTolerance_);
    }

    size_t _nLeftIndex = 1;
    size_t _nRightIndex = 1;
    while (_nLeftIndex < vecLeft_.size() && _nRightIndex < vecRight_.size()) {
        double _nLeftDist = DistPointToSegment(vecRight_[_nRightIndex - 1], vecRight_[_nRightIndex], vecLeft_[_nLeftIndex]);
        double _nRightDist = DistPointToSegment(vecLeft_[_nLeftIndex - 1], vecLeft_[_nLeftIndex], vecRight_[_nRightIndex]);
        if (Precision::IsAlmostEqual(_nLeftDist, _nRightDist, Precision::RealTolerance())) {
            if (vecLeft_.size() < vecRight_.size()) {
                ASSERT_NEAR(_nRightDist, 0.0, nTolerance_);
                ++_nRightIndex;
            }
            else {
                ASSERT_NEAR(_nLeftDist, 0.0, nTolerance_);
                ++_nLeftIndex;
            }
        }
        else if (_nLeftDist < _nRightDist) {
            ASSERT_NEAR(_nLeftDist, 0.0, nTolerance_);
            ++_nLeftIndex;
        }
        else {
            ASSERT_NEAR(_nRightDist, 0.0, nTolerance_);
            ++_nRightIndex;
        }
    }

    ASSERT_TRUE(_nLeftIndex == vecLeft_.size() || _nLeftIndex + 1 == vecLeft_.size());
    ASSERT_TRUE(_nRightIndex == vecRight_.size() || _nRightIndex + 1 == vecRight_.size());
}

static void CheckPolylinesSimilar(const Polyline2D& Polyline_, const VECDPOINT2& vecPoints_, double nTolerance_)
{
    // Convert polyline to points
    VECDPOINT2 _vecPoints;
    _vecPoints.emplace_back(DPOINT2(Polyline_.nStartX, Polyline_.nStartY));
    for (const auto& _node : Polyline_.vecNodes) {
        DPOINT2 _ptPoint(_node.nEndX, _node.nEndY);
        if (Precision::IsAlmostEqual(_node.nBulge, 0.0, Precision::RealTolerance())) { // line
            _vecPoints.emplace_back(_ptPoint);
        }
        else { // arc
            ArcNode2D _ArcNode;
            RetrieveArcNode2D(_vecPoints.back(), _ptPoint, _node.nBulge, _ArcNode);
            double _nTolerance = 0.5 * min(_ArcNode.nRadius, nTolerance_);
            double _nDeltaAngle = 2.0 * acos(1.0 - _nTolerance / _ArcNode.nRadius);
            int _nCount = static_cast<int>(ceil(fabs(_ArcNode.nEndAngle - _ArcNode.nStartAngle) / _nDeltaAngle));
            for (int i = 1; i <= _nCount; ++i) {
                double _nRatio = 1.0 * i / _nCount;
                double _nAngle = (1.0 - _nRatio) * _ArcNode.nStartAngle + _nRatio * _ArcNode.nEndAngle;
                _vecPoints.emplace_back(_ArcNode.ptCenter + _ArcNode.nRadius * DPOINT2(cos(_nAngle), sin(_nAngle)));
            }
        }
    }

    CheckPolylinesSimilar(_vecPoints, vecPoints_, nTolerance_);
}

static void CheckPolylinesSimilar(const VECDPOINT3& vecLeft_, const VECDPOINT3& vecRight_, double nTolerance_)
{
    // Check start and end coordinates
    for (int i = axis::x; i <= axis::z; ++i) {
        ASSERT_NEAR(vecLeft_.front()[i], vecRight_.front()[i], nTolerance_);
        ASSERT_NEAR(vecLeft_.back()[i], vecRight_.back()[i], nTolerance_);
    }

    size_t _nLeftIndex = 1;
    size_t _nRightIndex = 1;
    while (_nLeftIndex < vecLeft_.size() && _nRightIndex < vecRight_.size()) {
        double _nLeftDist = DistPointToSegment(vecRight_[_nRightIndex - 1], vecRight_[_nRightIndex], vecLeft_[_nLeftIndex]);
        double _nRightDist = DistPointToSegment(vecLeft_[_nLeftIndex - 1], vecLeft_[_nLeftIndex], vecRight_[_nRightIndex]);
        if (Precision::IsAlmostEqual(_nLeftDist, _nRightDist, Precision::RealTolerance())) {
            if (vecLeft_.size() < vecRight_.size()) {
                ASSERT_NEAR(_nRightDist, 0.0, nTolerance_);
                ++_nRightIndex;
            }
            else {
                ASSERT_NEAR(_nLeftDist, 0.0, nTolerance_);
                ++_nLeftIndex;
            }
        }
        else if (_nLeftDist < _nRightDist) {
            ASSERT_NEAR(_nLeftDist, 0.0, nTolerance_);
            ++_nLeftIndex;
        }
        else {
            ASSERT_NEAR(_nRightDist, 0.0, nTolerance_);
            ++_nRightIndex;
        }
    }

    ASSERT_TRUE(_nLeftIndex == vecLeft_.size() || _nLeftIndex + 1 == vecLeft_.size());
    ASSERT_TRUE(_nRightIndex == vecRight_.size() || _nRightIndex + 1 == vecRight_.size());
}

static void CheckPolylinesSimilar(const std::vector<ScatterNode2D>& vecNodes_, const VECDPOINT2& vecPoints_, double nTolerance_)
{
    // Extract scatter points
    VECDPOINT2 _vecPoints;
    for (const auto& _node : vecNodes_) {
        _vecPoints.push_back(_node.ptPoint);
    }

    CheckPolylinesSimilar(_vecPoints, vecPoints_, nTolerance_);
}

static void CheckPolylinesSimilar(const std::vector<ScatterNode3D>& vecNodes_, const VECDPOINT3& vecPoints_, double nTolerance_)
{
    // Extract scatter points
    VECDPOINT3 _vecPoints;
    for (const auto& _node : vecNodes_) {
        _vecPoints.push_back(_node.ptPoint);
    }

    CheckPolylinesSimilar(_vecPoints, vecPoints_, nTolerance_);
}

static double DistPointToSegment(const DPOINT2& ptStartPoint_, const DPOINT2& ptEndPoint_, const DPOINT2& ptAimPoint_)
{
    // First determine whether the point-to-line foot is in the line segment
    DPOINT2 _ptStartAim = ptAimPoint_ - ptStartPoint_;
    DPOINT2 _ptStartEnd = ptEndPoint_ - ptStartPoint_;
    DPOINT2 _ptAimEnd = ptEndPoint_ - ptAimPoint_;
    if (DotProduct(_ptStartAim, _ptStartEnd) < -1.0 * Precision::RealTolerance()
        || DotProduct(_ptStartEnd, _ptAimEnd) < -1.0 * Precision::RealTolerance()) {
        // Point outside line segment
        return min(_ptStartAim.GetLength(), _ptAimEnd.GetLength());
    }
    else {
        // Point in line segment
        return fabs(CrossProduct(_ptStartAim, _ptStartEnd) / _ptStartEnd.GetLength());
    }
}

static double DistPointToSegment(const DPOINT3& ptStartPoint_, const DPOINT3& ptEndPoint_, const DPOINT3& ptAimPoint_)
{
    // First determine whether the point-to-line foot is in the line segment
    DPOINT3 _ptStartAim = ptAimPoint_ - ptStartPoint_;
    DPOINT3 _ptStartEnd = ptEndPoint_ - ptStartPoint_;
    DPOINT3 _ptAimEnd = ptEndPoint_ - ptAimPoint_;
    if (DotProduct(_ptStartAim, _ptStartEnd) < -1.0 * Precision::RealTolerance()
        || DotProduct(_ptStartEnd, _ptAimEnd) < -1.0 * Precision::RealTolerance()) {
        // Point outside line segment
        return min(_ptStartAim.GetLength(), _ptAimEnd.GetLength());
    }
    else {
        // Point in line segment
        return CrossProduct(_ptStartAim, _ptStartEnd).GetLength() / _ptStartEnd.GetLength();
    }
}