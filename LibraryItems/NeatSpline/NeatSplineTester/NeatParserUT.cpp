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

/**
 * This file unit tests the interface for parsing splines.
 */
TEST(NeatParserUT, 1DLineNurbsNode)
{
    // One-dimensional line
    NurbsNode1D _NurbsNode;
    _NurbsNode.nDegree = 1;
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.5);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecPoles.push_back(92.926362318722781);
    _NurbsNode.vecPoles.push_back(63.9623694586018275);
    _NurbsNode.vecPoles.push_back(34.998376598480874);

    // Parser nurbs node
    CNurbsParser1D _NurbsParser;
    double _nPoint, _nDeriv1, _nDeriv2;
    EXPECT_NO_THROW(_NurbsParser.SetNurbsNode(&_NurbsNode));

    double _nxKnots[] = { 0.0, 0.5, 1.0 };
    double _nxAimPoints[] = { 92.926362318722781, 63.9623694586018275, 34.998376598480874 };
    for (int i = 0; i < _countof(_nxKnots); ++i)
    {
        double _nKnot = _nxKnots[i];
        double _nAimPoint = _nxAimPoints[i];
        EXPECT_NO_THROW(_NurbsParser.GetNurbsPoint(_nKnot, _nPoint));
        EXPECT_NEAR(_nPoint, _nAimPoint, Precision::RealTolerance());
        EXPECT_NO_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _nPoint, _nDeriv1));
        EXPECT_NEAR(_nPoint, _nAimPoint, Precision::RealTolerance());
        EXPECT_TRUE(_nDeriv1 < -Precision::RealTolerance());
        EXPECT_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _nPoint, _nDeriv1, _nDeriv2), std::exception);
    }
}

TEST(NeatParserUT, 2DLineNurbsNode)
{
    // Two-dimensional line
    NurbsNode2D _NurbsNode;
    _NurbsNode.nDegree = 1;
    _NurbsNode.vecKnots.push_back(0.32054737193021660);
    _NurbsNode.vecKnots.push_back(0.32054737193021660);
    _NurbsNode.vecKnots.push_back(0.32075471698113212);
    _NurbsNode.vecKnots.push_back(0.32704402515723269);
    _NurbsNode.vecKnots.push_back(0.32704402515723269);
    _NurbsNode.vecPoles.push_back(DPOINT2(7.1641142410169740, 3.8890870071682211));
    _NurbsNode.vecPoles.push_back(DPOINT2(7.0994634359756636, 4.7007347412309377));
    _NurbsNode.vecPoles.push_back(DPOINT2(5.1384383963248146, 29.320096078250600));

    // Parser nurbs node
    CNurbsParser2D _NurbsParser;
    DPOINT2 _ptPoint, _ptDeriv1, _ptDeriv2;
    EXPECT_NO_THROW(_NurbsParser.SetNurbsNode(&_NurbsNode));

    DPOINT2 _ptAimDeriv1 = _NurbsNode.vecPoles.back() - _NurbsNode.vecPoles.front();
    _ptAimDeriv1 /= _ptAimDeriv1.GetLength();
    double _nxKnots[] = { 0.32054737193021660, 0.32075471698113212, 0.32704402515723269 };
    DPOINT2 _ptAimPoints[] = { DPOINT2(7.1641142410169740, 3.8890870071682211),
        DPOINT2(7.0994634359756636, 4.7007347412309377),
        DPOINT2(5.1384383963248146, 29.320096078250600) };
    for (int i = 0; i < _countof(_nxKnots); ++i)
    {
        double _nKnot = _nxKnots[i];
        DPOINT2 _ptAimPoint = _ptAimPoints[i];
        EXPECT_NO_THROW(_NurbsParser.GetNurbsPoint(_nKnot, _ptPoint));
        CheckDPoint2Equal(_ptPoint, _ptAimPoint, Precision::RealTolerance());
        EXPECT_NO_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint, _ptDeriv1));
        CheckDPoint2Equal(_ptPoint, _ptAimPoint, Precision::RealTolerance());
        CheckDPoint2Equal(_ptDeriv1 / _ptDeriv1.GetLength(), _ptAimDeriv1, Precision::RealTolerance());
        EXPECT_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint, _ptDeriv1, _ptDeriv2), std::exception);
    }
}

TEST(NeatParserUT, 3DLineNurbsNode)
{
    // Three-dimensional line
    NurbsNode3D _NurbsNode;
    _NurbsNode.nDegree = 1;
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.5);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecPoles.push_back(DPOINT3(92.926362318722781, 19.659525043120819, 61.604467614663918));
    _NurbsNode.vecPoles.push_back(DPOINT3(63.9623694586018275, 22.3839554203619625, 54.466676252468421));
    _NurbsNode.vecPoles.push_back(DPOINT3(34.998376598480874, 25.108385797603106, 47.328884890272924));

    // Parser nurbs node
    CNurbsParser3D _NurbsParser;
    DPOINT3 _ptPoint, _ptDeriv1, _ptDeriv2;
    EXPECT_NO_THROW(_NurbsParser.SetNurbsNode(&_NurbsNode));

    DPOINT3 _ptAimDeriv1 = _NurbsNode.vecPoles.back() - _NurbsNode.vecPoles.front();
    _ptAimDeriv1 /= _ptAimDeriv1.GetLength();
    double _nxKnots[] = { 0.0, 0.5, 1.0 };
    DPOINT3 _ptAimPoints[] = { DPOINT3(92.926362318722781, 19.659525043120819, 61.604467614663918),
        DPOINT3(63.9623694586018275, 22.3839554203619625, 54.466676252468421),
        DPOINT3(34.998376598480874, 25.108385797603106, 47.328884890272924) };
    for (int i = 0; i < _countof(_nxKnots); ++i)
    {
        double _nKnot = _nxKnots[i];
        DPOINT3 _ptAimPoint = _ptAimPoints[i];
        EXPECT_NO_THROW(_NurbsParser.GetNurbsPoint(_nKnot, _ptPoint));
        CheckDPoint3Equal(_ptPoint, _ptAimPoint, Precision::RealTolerance());
        EXPECT_NO_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint, _ptDeriv1));
        CheckDPoint3Equal(_ptPoint, _ptAimPoint, Precision::RealTolerance());
        CheckDPoint3Equal(_ptDeriv1 / _ptDeriv1.GetLength(), _ptAimDeriv1, Precision::RealTolerance());
        EXPECT_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint, _ptDeriv1, _ptDeriv2), std::exception);
    }
}

TEST(NeatParserUT, 2DArcNurbsNode)
{
    // Two-dimensional arc
    NurbsNode2D _NurbsNode;
    _NurbsNode.nDegree = 2;
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.25);
    _NurbsNode.vecKnots.push_back(0.5);
    _NurbsNode.vecKnots.push_back(0.5);
    _NurbsNode.vecKnots.push_back(0.75);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);

    double _nRadius = 10.0;
    _NurbsNode.vecPoles.push_back(DPOINT2(_nRadius, 0.0));
    _NurbsNode.vecPoles.push_back(DPOINT2(_nRadius, _nRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nRadius, _nRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nRadius, 0.0));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nRadius, -_nRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(_nRadius, -_nRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(_nRadius, 0.0));
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(1.0);

    // Parser nurbs node
    CNurbsParser2D _NurbsParser;
    EXPECT_NO_THROW(_NurbsParser.SetNurbsNode(&_NurbsNode));

    DPOINT2 _ptPoint[3];
    DPOINT2 _ptDeriv1[2];
    DPOINT2 _ptDeriv2;
    double _nxKnots[] = { 0.0, 0.25, 0.5, 0.75, 1.0 };
    DPOINT2 _ptAimPoints[] = { DPOINT2(_nRadius, 0.0), DPOINT2(0.0, _nRadius), DPOINT2(-_nRadius, 0.0),
        DPOINT2(0.0, -_nRadius), DPOINT2(_nRadius, 0.0) };
    for (int i = 0; i < _countof(_nxKnots); ++i)
    {
        double _nKnot = _nxKnots[i];
        DPOINT2 _ptAimPoint = _ptAimPoints[i];
        EXPECT_NO_THROW(_NurbsParser.GetNurbsPoint(_nKnot, _ptPoint[0]));
        EXPECT_NO_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint[1], _ptDeriv1[0]));
        EXPECT_NO_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint[2], _ptDeriv1[1], _ptDeriv2));
        CheckDPoint2Equal(_ptAimPoint, _ptPoint[0], Precision::RealTolerance());
        CheckDPoint2Equal(_ptAimPoint, _ptPoint[1], Precision::RealTolerance());
        CheckDPoint2Equal(_ptAimPoint, _ptPoint[2], Precision::RealTolerance());
        CheckDPoint2Equal(_ptDeriv1[0], _ptDeriv1[1], Precision::RealTolerance());
        double _nOuterProduct = CrossProduct(_ptDeriv1[1], _ptDeriv2);
        double _nCurvature = fabs(_nOuterProduct) / pow(_ptDeriv1[1].GetLength(), 3);
        EXPECT_NEAR(_nCurvature, 1.0 / _nRadius, Precision::RealTolerance());
    }
}

TEST(NeatParserUT, 3DArcNurbsNode)
{
    // Three-dimensional arc: a third-order Bezier curve on a two-dimensional plane is fitted to a 90-degree arc,
    // and then transformed by rotating around the Y axis
    const double _nCosAngle = cos(Precision::PIE() / 6.0);
    const double _nSinAngle = sin(Precision::PIE() / 6.0);

    NurbsNode3D _NurbsNode;
    _NurbsNode.nDegree = 3;
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);

    double _nRadius = 10.0;
    double _nHeight = 4.0 / 3.0 * _nRadius * (1 - cos(Precision::PIE() / 4.0)) / sin(Precision::PIE() / 4.0);
    double _nFitError = 0.001 * _nRadius;
    _NurbsNode.vecPoles.push_back(DPOINT3(_nRadius * _nCosAngle, 0.0, _nRadius * _nSinAngle));
    _NurbsNode.vecPoles.push_back(DPOINT3(_nRadius * _nCosAngle, _nHeight, _nRadius * _nSinAngle));
    _NurbsNode.vecPoles.push_back(DPOINT3(_nHeight * _nCosAngle, _nRadius, _nHeight * _nSinAngle));
    _NurbsNode.vecPoles.push_back(DPOINT3(0.0, _nRadius, 0.0));

    // Parser nurbs node
    CNurbsParser3D _NurbsParser;
    EXPECT_NO_THROW(_NurbsParser.SetNurbsNode(&_NurbsNode));

    DPOINT3 _ptPoint[3];
    DPOINT3 _ptDeriv1[2];
    DPOINT3 _ptDeriv2;
    double _nxKnots[] = { 0.0, 0.5, 1.0 };
    double _nSideLength = _nRadius / 2.0 + 3.0 / 8.0 * _nHeight;
    DPOINT3 _ptAimPoints[] = { DPOINT3(_nRadius * _nCosAngle, 0.0, _nRadius * _nSinAngle),
        DPOINT3(_nSideLength * _nCosAngle, _nSideLength, _nSideLength * _nSinAngle),
        DPOINT3(0.0, _nRadius, 0.0) };
    double _nxCurvatures[] = { 2.0 / 3.0 * (_nRadius - _nHeight) / pow(_nHeight, 2),
        16.0 / (3.0 * sqrt(2.0)) * _nHeight / pow(2.0 * _nRadius - _nHeight, 2),
        2.0 / 3.0 * (_nRadius - _nHeight) / pow(_nHeight, 2) };
    for (int i = 0; i < _countof(_nxKnots); ++i)
    {
        double _nKnot = _nxKnots[i];
        DPOINT3 _ptAimPoint = _ptAimPoints[i];
        EXPECT_NO_THROW(_NurbsParser.GetNurbsPoint(_nKnot, _ptPoint[0]));
        EXPECT_NO_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint[1], _ptDeriv1[0]));
        EXPECT_NO_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint[2], _ptDeriv1[1], _ptDeriv2));
        CheckDPoint3Equal(_ptAimPoint, _ptPoint[0], Precision::RealTolerance());
        CheckDPoint3Equal(_ptAimPoint, _ptPoint[1], Precision::RealTolerance());
        CheckDPoint3Equal(_ptAimPoint, _ptPoint[2], Precision::RealTolerance());
        CheckDPoint3Equal(_ptDeriv1[0], _ptDeriv1[1], Precision::RealTolerance());
        DPOINT3 _nxOuterProduct = CrossProduct(_ptDeriv1[1], _ptDeriv2);
        double _nCurvature = _nxOuterProduct.GetLength() / pow(_ptDeriv1[1].GetLength(), 3);
        EXPECT_NEAR(_nCurvature, _nxCurvatures[i], Precision::RealTolerance());
    }
}

TEST(NeatParserUT, 2DEllipseNurbsNode)
{
    // Two-dimensional ellipse
    NurbsNode2D _NurbsNode;
    _NurbsNode.nDegree = 2;
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(1.0 / 3.0);
    _NurbsNode.vecKnots.push_back(2.0 / 3.0);
    _NurbsNode.vecKnots.push_back(2.0 / 3.0);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);

    double _nLongRadius = 200.0;
    double _nShortRadius = 100.0;
    _NurbsNode.vecPoles.push_back(DPOINT2(_nLongRadius, 0.0));
    _NurbsNode.vecPoles.push_back(DPOINT2(_nLongRadius, -_nShortRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nLongRadius, -_nShortRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nLongRadius, 0.0));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nLongRadius, _nShortRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(0.0, _nShortRadius));
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(sqrt(2.0) / 2.0);
    _NurbsNode.vecWeights.push_back(1.0);

    // Parser nurbs node
    CNurbsParser2D _NurbsParser;
    EXPECT_NO_THROW(_NurbsParser.SetNurbsNode(&_NurbsNode));

    DPOINT2 _ptPoint[3];
    DPOINT2 _ptDeriv1[2];
    DPOINT2 _ptDeriv2;
    double _nxKnots[] = { 0.0, 1.0 / 3.0, 2.0 / 3.0, 1.0 };
    DPOINT2 _ptAimPoints[] = { DPOINT2(_nLongRadius, 0.0),
        DPOINT2(0.0, -_nShortRadius),
        DPOINT2(-_nLongRadius, 0.0),
        DPOINT2(0.0, _nShortRadius) };
    double _nxCurvatures[] = { _nLongRadius / (_nShortRadius * _nShortRadius),
        _nShortRadius / (_nLongRadius * _nLongRadius),
        _nLongRadius / (_nShortRadius * _nShortRadius),
        _nShortRadius / (_nLongRadius * _nLongRadius) };
    for (int i = 0; i < _countof(_nxKnots); ++i)
    {
        double _nKnot = _nxKnots[i];
        DPOINT2 _ptAimPoint = _ptAimPoints[i];
        EXPECT_NO_THROW(_NurbsParser.GetNurbsPoint(_nKnot, _ptPoint[0]));
        EXPECT_NO_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint[1], _ptDeriv1[0]));
        EXPECT_NO_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint[2], _ptDeriv1[1], _ptDeriv2));
        CheckDPoint2Equal(_ptAimPoint, _ptPoint[0], Precision::RealTolerance());
        CheckDPoint2Equal(_ptAimPoint, _ptPoint[1], Precision::RealTolerance());
        CheckDPoint2Equal(_ptAimPoint, _ptPoint[2], Precision::RealTolerance());
        CheckDPoint2Equal(_ptDeriv1[0], _ptDeriv1[1], Precision::RealTolerance());
        double _nOuterProduct = CrossProduct(_ptDeriv1[1], _ptDeriv2);
        double _nCurvature = fabs(_nOuterProduct) / pow(_ptDeriv1[1].GetLength(), 3);
        EXPECT_NEAR(_nCurvature, _nxCurvatures[i], Precision::RealTolerance());
    }
}

TEST(NeatParserUT, 3DEllipseNurbsNode)
{
    // Three-dimensional ellipse: a semi-ellipse on a two-dimensional plane,
    // and then obtained by rotating transformation around the X axis
    const double _nCosAngle = cos(Precision::PIE() / 3.0);
    const double _nSinAngle = sin(Precision::PIE() / 3.0);

    NurbsNode3D _NurbsNode;
    _NurbsNode.nDegree = 3;
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);

    double _nLongRadius = 60.0;
    double _nShortRadius = 40.0;
    _NurbsNode.vecPoles.push_back(DPOINT3(_nLongRadius, 0.0, 0.0));
    _NurbsNode.vecPoles.push_back(DPOINT3(_nLongRadius, 2.0 * _nShortRadius * _nCosAngle, -2.0 * _nShortRadius * _nSinAngle));
    _NurbsNode.vecPoles.push_back(DPOINT3(-_nLongRadius, 2.0 * _nShortRadius * _nCosAngle, -2.0 * _nShortRadius * _nSinAngle));
    _NurbsNode.vecPoles.push_back(DPOINT3(-_nLongRadius, 0.0, 0.0));
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(1.0 / 3.0);
    _NurbsNode.vecWeights.push_back(1.0 / 3.0);
    _NurbsNode.vecWeights.push_back(1.0);

    // Parser nurbs node
    CNurbsParser3D _NurbsParser;
    EXPECT_NO_THROW(_NurbsParser.SetNurbsNode(&_NurbsNode));

    DPOINT3 _ptPoint[3];
    DPOINT3 _ptDeriv1[2];
    DPOINT3 _ptDeriv2;
    double _nxKnots[] = { 0.0, 0.5, 1.0 };
    DPOINT3 _ptAimPoints[] = { DPOINT3(_nLongRadius, 0.0, 0.0),
        DPOINT3(0.0, _nShortRadius * _nCosAngle, -_nShortRadius * _nSinAngle),
        DPOINT3(-_nLongRadius, 0.0, 0.0) };
    double _nxCurvatures[] = { _nLongRadius / (_nShortRadius * _nShortRadius),
        _nShortRadius / (_nLongRadius * _nLongRadius),
        _nLongRadius / (_nShortRadius * _nShortRadius) };
    for (int i = 0; i < _countof(_nxKnots); ++i)
    {
        double _nKnot = _nxKnots[i];
        DPOINT3 _ptAimPoint = _ptAimPoints[i];
        EXPECT_NO_THROW(_NurbsParser.GetNurbsPoint(_nKnot, _ptPoint[0]));
        EXPECT_NO_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint[1], _ptDeriv1[0]));
        EXPECT_NO_THROW(_NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint[2], _ptDeriv1[1], _ptDeriv2));
        CheckDPoint3Equal(_ptAimPoint, _ptPoint[0], Precision::RealTolerance());
        CheckDPoint3Equal(_ptAimPoint, _ptPoint[1], Precision::RealTolerance());
        CheckDPoint3Equal(_ptAimPoint, _ptPoint[2], Precision::RealTolerance());
        CheckDPoint3Equal(_ptDeriv1[0], _ptDeriv1[1], Precision::RealTolerance());
        DPOINT3 _nxOuterProduct = CrossProduct(_ptDeriv1[1], _ptDeriv2);
        double _nCurvature = _nxOuterProduct.GetLength() / pow(_ptDeriv1[1].GetLength(), 3);
        EXPECT_NEAR(_nCurvature, _nxCurvatures[i], Precision::RealTolerance());
    }
}