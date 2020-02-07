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

using namespace neat;
using namespace tester;

/**
 * This file unit tests the interface for fitting splines.
 */
TEST(NeatFitterUT, 1DNurbsNode)
{
    // knot vector
    const double _szSumLengths[] = { 0.0, 0.0, 0.0, 0.0, 50.0, 50.0, 50.0, 50.0 };
    const double _szLimitSpeeds[] = { 0.0, 50.0 };
    const double _szAimPoles[] = { 0.0, 50.0 / 3.0, 100.0 / 3.0, 50.0 };

    // One-dimensional nurbs node
    VECDOUBLE _vecSumLengths;
    for (int i = 0; i < _countof(_szSumLengths); ++i) {
        _vecSumLengths.push_back(_szSumLengths[i]);
    }

    VECDOUBLE _vecLimitSpeeds;
    for (int i = 0; i < _countof(_szLimitSpeeds); ++i) {
        _vecLimitSpeeds.push_back(_szLimitSpeeds[i]);
    }

    // Interpolated nurbs node
    CNurbsInterp1D _NurbsInterp;
    NurbsNode1D _NurbsNode;
    EXPECT_THROW(_NurbsInterp.InterpNurbsCurve(_vecLimitSpeeds, _NurbsNode), std::exception);

    EXPECT_NO_THROW(_NurbsInterp.SetKnotVector(_vecSumLengths));
    EXPECT_TRUE(_NurbsInterp.InterpNurbsCurve(_vecLimitSpeeds, _NurbsNode));

    // Check nurbs node
    ASSERT_TRUE(_NurbsNode.vecWeights.empty());
    ASSERT_EQ(_NurbsNode.nDegree, 3);
    ASSERT_EQ(_NurbsNode.vecPoles.size(), _countof(_szAimPoles));
    for (int i = 0; i < _countof(_szAimPoles); ++i) {
        EXPECT_NEAR(_NurbsNode.vecPoles[i], _szAimPoles[i], Precision::RealTolerance());
    }
}

TEST(NeatFitterUT, 2DNurbsNodeNoLimit)
{
    const DPOINT2 _szFitPoints[] = { DPOINT2(0.0, 0.0), DPOINT2(10.0, 10.0), DPOINT2(20.0, 0.0) };
    const double _szAimKnotVector[] = { 0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0 };
    const DPOINT2 _szAimPoles[] = { DPOINT2(0.0, 0.0), DPOINT2(10.0 / 3.0, 5.0), DPOINT2(10.0, 15.0),
        DPOINT2(50.0 / 3.0, 5.0), DPOINT2(20.0, 0.0) };

    // Two-dimensional nurbs node
    VECDPOINT2 _vecFitPoints;
    for (int i = 0; i < _countof(_szFitPoints); ++i) {
        _vecFitPoints.push_back(_szFitPoints[i]);
    }

    // Interpolated nurbs node
    CNurbsInterp2D _NurbsInterp;
    NurbsNode2D _NurbsNode;
    EXPECT_TRUE(_NurbsInterp.InterpNurbsCurve(_vecFitPoints, _NurbsNode));

    // Check nurbs node
    ASSERT_TRUE(_NurbsNode.vecWeights.empty());
    ASSERT_EQ(_NurbsNode.nDegree, 3);
    ASSERT_EQ(_NurbsNode.vecKnots.size(), _countof(_szAimKnotVector));
    ASSERT_EQ(_NurbsNode.vecPoles.size(), _countof(_szAimPoles));
    for (int i = 0; i < _countof(_szAimKnotVector); ++i) {
        EXPECT_NEAR(_NurbsNode.vecKnots[i], _szAimKnotVector[i], Precision::RealTolerance());
    }

    for (int i = 0; i < _countof(_szAimPoles); ++i) {
        CheckDPoint2Equal(_NurbsNode.vecPoles[i], _szAimPoles[i], Precision::RealTolerance());
    }
}

TEST(NeatFitterUT, 2DNurbsNodeWithTangent)
{
    const DPOINT2 _szFitPoints[] = { DPOINT2(0.0, 0.0), DPOINT2(10.0, 10.0), DPOINT2(20.0, 0.0) };
    const double _szAimKnotVector[] = { 0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0 };
    const DPOINT2 _szAimPoles[] = { DPOINT2(0.0, 0.0), DPOINT2(10.0 / 3.0, 5.0), DPOINT2(10.0, 15.0),
        DPOINT2(50.0 / 3.0, 5.0), DPOINT2(20.0, 0.0) };
    DPOINT2 _ptStartTangent(20.0, 30.0);
    DPOINT2 _ptEndTangent(20.0, -30.0);

    // Two-dimensional nurbs node
    VECDPOINT2 _vecFitPoints;
    for (int i = 0; i < _countof(_szFitPoints); ++i) {
        _vecFitPoints.push_back(_szFitPoints[i]);
    }

    // Interpolated nurbs node
    CNurbsInterp2D _NurbsInterp;
    NurbsNode2D _NurbsNode;
    EXPECT_NO_THROW(_NurbsInterp.SetStartTangent(_ptStartTangent));
    EXPECT_NO_THROW(_NurbsInterp.SetEndTangent(_ptEndTangent));
    EXPECT_TRUE(_NurbsInterp.InterpNurbsCurve(_vecFitPoints, _NurbsNode));

    // Check nurbs node
    ASSERT_TRUE(_NurbsNode.vecWeights.empty());
    ASSERT_EQ(_NurbsNode.nDegree, 3);
    ASSERT_EQ(_NurbsNode.vecKnots.size(), _countof(_szAimKnotVector));
    ASSERT_EQ(_NurbsNode.vecPoles.size(), _countof(_szAimPoles));
    for (int i = 0; i < _countof(_szAimKnotVector); ++i) {
        EXPECT_NEAR(_NurbsNode.vecKnots[i], _szAimKnotVector[i], Precision::RealTolerance());
    }

    for (int i = 0; i < _countof(_szAimPoles); ++i) {
        CheckDPoint2Equal(_NurbsNode.vecPoles[i], _szAimPoles[i], Precision::RealTolerance());
    }
}

TEST(NeatFitterUT, 2DNurbsNodeWithKnotVector)
{
    const DPOINT2 _szFitPoints[] = { DPOINT2(0.0, 0.0), DPOINT2(10.0, 10.0), DPOINT2(20.0, 0.0) };
    const double _szKnotVector[] = { 0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0 };
    const DPOINT2 _szAimPoles[] = { DPOINT2(0.0, 0.0), DPOINT2(10.0 / 3.0, 5.0), DPOINT2(10.0, 15.0),
        DPOINT2(50.0 / 3.0, 5.0), DPOINT2(20.0, 0.0) };

    // Two-dimensional nurbs node
    VECDOUBLE _vecKnotVector;
    for (int i = 0; i < _countof(_szKnotVector); ++i) {
        _vecKnotVector.push_back(_szKnotVector[i]);
    }

    VECDPOINT2 _vecFitPoints;
    for (int i = 0; i < _countof(_szFitPoints); ++i) {
        _vecFitPoints.push_back(_szFitPoints[i]);
    }

    // Interpolated nurbs node
    CNurbsInterp2D _NurbsInterp;
    NurbsNode2D _NurbsNode;
    EXPECT_NO_THROW(_NurbsInterp.SetKnotVector(_vecKnotVector));
    EXPECT_TRUE(_NurbsInterp.InterpNurbsCurve(_vecFitPoints, _NurbsNode));

    // Check nurbs node
    ASSERT_TRUE(_NurbsNode.vecWeights.empty());
    ASSERT_EQ(_NurbsNode.nDegree, 3);
    ASSERT_EQ(_NurbsNode.vecPoles.size(), _countof(_szAimPoles));
    for (int i = 0; i < _countof(_szAimPoles); ++i) {
        CheckDPoint2Equal(_NurbsNode.vecPoles[i], _szAimPoles[i], Precision::RealTolerance());
    }
}

TEST(NeatFitterUT, 3DNurbsNodeNoLimit)
{
    // Rotate and transform the 2D plane data around the X axis
    const double _nCosAngle = cos(Precision::PIE() / 6.0);
    const double _nSinAngle = sin(Precision::PIE() / 6.0);
    const DPOINT3 _szFitPoints[] = {
        DPOINT3(0.0, 0.0, 0.0),
        DPOINT3(10.0, 10.0 * _nCosAngle, 10.0 * _nSinAngle),
        DPOINT3(20.0, 0.0, 0.0) };
    const double _szAimKnotVector[] = { 0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0 };
    const DPOINT3 _szAimPoles[] = {
        DPOINT3(0.0, 0.0, 0.0),
        DPOINT3(10.0 / 3.0, 5.0 * _nCosAngle, 5.0 * _nSinAngle),
        DPOINT3(10.0, 15.0 * _nCosAngle, 15.0 * _nSinAngle),
        DPOINT3(50.0 / 3.0, 5.0 * _nCosAngle, 5.0 * _nSinAngle),
        DPOINT3(20.0, 0.0, 0.0) };

    // Three-dimensional nurbs node
    VECDPOINT3 _vecFitPoints;
    for (int i = 0; i < _countof(_szFitPoints); ++i) {
        _vecFitPoints.push_back(_szFitPoints[i]);
    }

    // Interpolated nurbs node
    CNurbsInterp3D _NurbsInterp;
    NurbsNode3D _NurbsNode;
    EXPECT_TRUE(_NurbsInterp.InterpNurbsCurve(_vecFitPoints, _NurbsNode));

    // Check nurbs node
    ASSERT_TRUE(_NurbsNode.vecWeights.empty());
    ASSERT_EQ(_NurbsNode.nDegree, 3);
    ASSERT_EQ(_NurbsNode.vecKnots.size(), _countof(_szAimKnotVector));
    ASSERT_EQ(_NurbsNode.vecPoles.size(), _countof(_szAimPoles));
    for (int i = 0; i < _countof(_szAimKnotVector); ++i) {
        EXPECT_NEAR(_NurbsNode.vecKnots[i], _szAimKnotVector[i], Precision::RealTolerance());
    }

    for (int i = 0; i < _countof(_szAimPoles); ++i) {
        CheckDPoint3Equal(_NurbsNode.vecPoles[i], _szAimPoles[i], Precision::RealTolerance());
    }
}

TEST(NeatFitterUT, 3DNurbsNodeWithTangent)
{
    // Rotate and transform the 2D plane data around the Y axis
    const double _nCosAngle = cos(Precision::PIE() / 3.0);
    const double _nSinAngle = sin(Precision::PIE() / 3.0);
    const DPOINT3 _szFitPoints[] = {
        DPOINT3(0.0, 0.0, 0.0),
        DPOINT3(10.0 * _nCosAngle, 10.0, -10.0 * _nSinAngle),
        DPOINT3(20.0 * _nCosAngle, 0.0, -20.0 * _nSinAngle) };
    const double _szAimKnotVector[] = { 0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0 };
    const DPOINT3 _szAimPoles[] = {
        DPOINT3(0.0, 0.0, 0.0),
        DPOINT3(10.0 / 3.0 * _nCosAngle, 5.0, -10.0 / 3.0 * _nSinAngle),
        DPOINT3(10.0 * _nCosAngle, 15.0, -10.0 * _nSinAngle),
        DPOINT3(50.0 / 3.0 * _nCosAngle, 5.0, -50.0 / 3.0 * _nSinAngle),
        DPOINT3(20.0 * _nCosAngle, 0.0, -20.0 * _nSinAngle) };
    DPOINT3 _ptStartTangent(20.0 * _nCosAngle, 30.0, -20.0 * _nSinAngle);
    DPOINT3 _ptEndTangent(20.0 * _nCosAngle, -30.0, -20.0 * _nSinAngle);

    // Three-dimensional nurbs node
    VECDPOINT3 _vecFitPoints;
    for (int i = 0; i < _countof(_szFitPoints); ++i) {
        _vecFitPoints.push_back(_szFitPoints[i]);
    }

    // Interpolated nurbs node
    CNurbsInterp3D _NurbsInterp;
    NurbsNode3D _NurbsNode;
    EXPECT_NO_THROW(_NurbsInterp.SetStartTangent(_ptStartTangent));
    EXPECT_NO_THROW(_NurbsInterp.SetEndTangent(_ptEndTangent));
    EXPECT_TRUE(_NurbsInterp.InterpNurbsCurve(_vecFitPoints, _NurbsNode));

    // Check nurbs node
    ASSERT_TRUE(_NurbsNode.vecWeights.empty());
    ASSERT_EQ(_NurbsNode.nDegree, 3);
    ASSERT_EQ(_NurbsNode.vecKnots.size(), _countof(_szAimKnotVector));
    ASSERT_EQ(_NurbsNode.vecPoles.size(), _countof(_szAimPoles));
    for (int i = 0; i < _countof(_szAimKnotVector); ++i) {
        EXPECT_NEAR(_NurbsNode.vecKnots[i], _szAimKnotVector[i], Precision::RealTolerance());
    }

    for (int i = 0; i < _countof(_szAimPoles); ++i) {
        CheckDPoint3Equal(_NurbsNode.vecPoles[i], _szAimPoles[i], Precision::RealTolerance());
    }
}

TEST(NeatFitterUT, 3DNurbsNodeWithKnotVector)
{
    // Rotate and transform the 2D plane data around the X axis
    const double _nCosAngle = cos(Precision::PIE() / 4.0);
    const double _nSinAngle = sin(Precision::PIE() / 4.0);
    const DPOINT3 _szFitPoints[] = {
        DPOINT3(0.0, 0.0, 0.0),
        DPOINT3(10.0, 10.0 * _nCosAngle, 10.0 * _nSinAngle),
        DPOINT3(20.0, 0.0, 0.0) };
    const double _szKnotVector[] = { 0.0, 0.0, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0, 1.0 };
    const DPOINT3 _szAimPoles[] = {
        DPOINT3(0.0, 0.0, 0.0),
        DPOINT3(10.0 / 3.0, 5.0 * _nCosAngle, 5.0 * _nSinAngle),
        DPOINT3(10.0, 15.0 * _nCosAngle, 15.0 * _nSinAngle),
        DPOINT3(50.0 / 3.0, 5.0 * _nCosAngle, 5.0 * _nSinAngle),
        DPOINT3(20.0, 0.0, 0.0) };

    // Three-dimensional nurbs node
    VECDOUBLE _vecKnotVector;
    for (int i = 0; i < _countof(_szKnotVector); ++i) {
        _vecKnotVector.push_back(_szKnotVector[i]);
    }

    VECDPOINT3 _vecFitPoints;
    for (int i = 0; i < _countof(_szFitPoints); ++i) {
        _vecFitPoints.push_back(_szFitPoints[i]);
    }

    // Interpolated nurbs node
    CNurbsInterp3D _NurbsInterp;
    NurbsNode3D _NurbsNode;
    EXPECT_NO_THROW(_NurbsInterp.SetKnotVector(_vecKnotVector));
    EXPECT_TRUE(_NurbsInterp.InterpNurbsCurve(_vecFitPoints, _NurbsNode));

    // Check nurbs node
    ASSERT_TRUE(_NurbsNode.vecWeights.empty());
    ASSERT_EQ(_NurbsNode.nDegree, 3);
    ASSERT_EQ(_NurbsNode.vecPoles.size(), _countof(_szAimPoles));
    for (int i = 0; i < _countof(_szAimPoles); ++i) {
        CheckDPoint3Equal(_NurbsNode.vecPoles[i], _szAimPoles[i], Precision::RealTolerance());
    }
}