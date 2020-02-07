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
 * This file unit tests the interface for scatting splines.
 */
TEST(NeatScatterUT, 2DLineNurbsNode)
{
    // Two-dimensional line
    NurbsNode2D _NurbsNode;
    _NurbsNode.nDegree = 1;
    _NurbsNode.vecKnots.push_back(0.32054737193021660);
    _NurbsNode.vecKnots.push_back(0.32054737193021660);
    _NurbsNode.vecKnots.push_back(0.32075471698113212);
    _NurbsNode.vecKnots.push_back(0.32704402515723269);
    _NurbsNode.vecKnots.push_back(0.32704402515723269);

    DPOINT2 _ptStartPoint(7.1641142410169740, 3.8890870071682211);
    DPOINT2 _ptEndPoint(5.1384383963248146, 29.320096078250600);
    _NurbsNode.vecPoles.push_back(_ptStartPoint);
    _NurbsNode.vecPoles.push_back(DPOINT2(7.0994634359756636, 4.7007347412309377));
    _NurbsNode.vecPoles.push_back(_ptEndPoint);

    // Scatter nurbs node
    const double _nDeflection = 0.001;
    VECDPOINT2 _vecScatterPoints;
    EXPECT_NO_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterPoints));
    EXPECT_EQ(_vecScatterPoints.size(), 2);
    CheckDPoint2Equal(_vecScatterPoints.front(), _ptStartPoint, Precision::RealTolerance());
    CheckDPoint2Equal(_vecScatterPoints.back(), _ptEndPoint, Precision::RealTolerance());

    std::vector<ScatterNode2D> _vecScatterNodes;
    EXPECT_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterNodes), std::exception);
}

TEST(NeatScatterUT, 3DLineNurbsNode)
{
    // Three-dimensional line
    NurbsNode3D _NurbsNode;
    _NurbsNode.nDegree = 1;
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.0);
    _NurbsNode.vecKnots.push_back(0.5);
    _NurbsNode.vecKnots.push_back(1.0);
    _NurbsNode.vecKnots.push_back(1.0);

    DPOINT3 _ptStartPoint(92.926362318722781, 19.659525043120819, 61.604467614663918);
    DPOINT3 _ptEndPoint(34.998376598480874, 25.108385797603106, 47.328884890272924);
    _NurbsNode.vecPoles.push_back(_ptStartPoint);
    _NurbsNode.vecPoles.push_back(DPOINT3(63.9623694586018275, 22.3839554203619625, 54.466676252468421));
    _NurbsNode.vecPoles.push_back(_ptEndPoint);

    // Scatter nurbs node
    const double _nDeflection = 0.001;
    VECDPOINT3 _vecScatterPoints;
    EXPECT_NO_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterPoints));
    EXPECT_EQ(_vecScatterPoints.size(), 2);
    CheckDPoint3Equal(_vecScatterPoints.front(), _ptStartPoint, Precision::RealTolerance());
    CheckDPoint3Equal(_vecScatterPoints.back(), _ptEndPoint, Precision::RealTolerance());

    std::vector<ScatterNode3D> _vecScatterNodes;
    EXPECT_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterNodes), std::exception);
}

TEST(NeatScatterUT, 2DArcNurbsNode)
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

    double _nRadius = 100.0;
    DPOINT2 _ptStartPoint(_nRadius, 0.0);
    DPOINT2 _ptEndPoint(_nRadius, 0.0);
    _NurbsNode.vecPoles.push_back(_ptStartPoint);
    _NurbsNode.vecPoles.push_back(DPOINT2(_nRadius, _nRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nRadius, _nRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nRadius, 0.0));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nRadius, -_nRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(_nRadius, -_nRadius));
    _NurbsNode.vecPoles.push_back(_ptEndPoint);
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(1.0);

    // Scatter nurbs node
    const double _nDeflection = 0.001;
    VECDPOINT2 _vecScatterPoints;
    EXPECT_NO_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterPoints));
    EXPECT_GT((int)_vecScatterPoints.size(), 1);
    CheckDPoint2Equal(_vecScatterPoints.front(), _ptStartPoint, Precision::RealTolerance());
    CheckDPoint2Equal(_vecScatterPoints.back(), _ptEndPoint, Precision::RealTolerance());
    for (size_t i = 0, _nSize = _vecScatterPoints.size(); i < _nSize; ++i) {
        double _nLength = _vecScatterPoints[i].GetLength();
        EXPECT_NEAR(_nLength, _nRadius, Precision::RealTolerance());
    }

    std::vector<ScatterNode2D> _vecScatterNodes;
    EXPECT_NO_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterNodes));
    EXPECT_GT((int)_vecScatterNodes.size(), 1);
    EXPECT_NEAR(_vecScatterNodes.front().nKnot, _NurbsNode.vecKnots.front(), Precision::RealTolerance());
    EXPECT_NEAR(_vecScatterNodes.back().nKnot, _NurbsNode.vecKnots.back(), Precision::RealTolerance());
    CheckDPoint2Equal(_vecScatterNodes.front().ptPoint, _ptStartPoint, Precision::RealTolerance());
    CheckDPoint2Equal(_vecScatterNodes.back().ptPoint, _ptEndPoint, Precision::RealTolerance());
    for (size_t i = 0, _nSize = _vecScatterNodes.size(); i < _nSize; ++i) {
        double _nLength = _vecScatterNodes[i].ptPoint.GetLength();
        EXPECT_NEAR(_nLength, _nRadius, Precision::RealTolerance());
        EXPECT_GE(_vecScatterNodes[i].ptDeriv1.GetLength(), Precision::RealTolerance());
        double _nOuterProduct = CrossProduct(_vecScatterNodes[i].ptDeriv1, _vecScatterNodes[i].ptDeriv2);
        double _nCurvature = fabs(_nOuterProduct) / pow(_vecScatterNodes[i].ptDeriv1.GetLength(), 3);
        EXPECT_NEAR(_nCurvature, 1.0 / _nRadius, Precision::RealTolerance());
    }
}

TEST(NeatScatterUT, 3DArcNurbsNode)
{
    // Three-dimensional arc: An arc on a two-dimensional plane, obtained by rotating and transforming around the Y axis
    const double _nCosAngle = cos(Precision::PIE() / 3.0);
    const double _nSinAngle = sin(Precision::PIE() / 3.0);

    NurbsNode3D _NurbsNode;
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

    double _nRadius = 30.0;
    DPOINT3 _ptStartPoint(_nRadius * _nCosAngle, 0.0, _nRadius * _nSinAngle);
    DPOINT3 _ptEndPoint(_nRadius * _nCosAngle, 0.0, _nRadius * _nSinAngle);
    _NurbsNode.vecPoles.push_back(_ptStartPoint);
    _NurbsNode.vecPoles.push_back(DPOINT3(_nRadius * _nCosAngle, _nRadius, _nRadius * _nSinAngle));
    _NurbsNode.vecPoles.push_back(DPOINT3(-_nRadius * _nCosAngle, _nRadius, -_nRadius * _nSinAngle));
    _NurbsNode.vecPoles.push_back(DPOINT3(-_nRadius * _nCosAngle, 0.0, -_nRadius * _nSinAngle));
    _NurbsNode.vecPoles.push_back(DPOINT3(-_nRadius * _nCosAngle, -_nRadius, -_nRadius * _nSinAngle));
    _NurbsNode.vecPoles.push_back(DPOINT3(_nRadius * _nCosAngle, -_nRadius, _nRadius * _nSinAngle));
    _NurbsNode.vecPoles.push_back(_ptEndPoint);
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(1.0);

    // Scatter nurbs node
    const double _nDeflection = 0.001;
    VECDPOINT3 _vecScatterPoints;
    EXPECT_NO_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterPoints));
    EXPECT_GT((int)_vecScatterPoints.size(), 1);
    CheckDPoint3Equal(_vecScatterPoints.front(), _ptStartPoint, Precision::RealTolerance());
    CheckDPoint3Equal(_vecScatterPoints.back(), _ptEndPoint, Precision::RealTolerance());
    for (size_t i = 0, _nSize = _vecScatterPoints.size(); i < _nSize; ++i) {
        double _nLength = _vecScatterPoints[i].GetLength();
        EXPECT_NEAR(_nLength, _nRadius, Precision::RealTolerance());
    }

    std::vector<ScatterNode3D> _vecScatterNodes;
    EXPECT_NO_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterNodes));
    EXPECT_GT((int)_vecScatterNodes.size(), 1);
    EXPECT_NEAR(_vecScatterNodes.front().nKnot, _NurbsNode.vecKnots.front(), Precision::RealTolerance());
    EXPECT_NEAR(_vecScatterNodes.back().nKnot, _NurbsNode.vecKnots.back(), Precision::RealTolerance());
    CheckDPoint3Equal(_vecScatterNodes.front().ptPoint, _ptStartPoint, Precision::RealTolerance());
    CheckDPoint3Equal(_vecScatterNodes.back().ptPoint, _ptEndPoint, Precision::RealTolerance());
    for (size_t i = 0, _nSize = _vecScatterNodes.size(); i < _nSize; ++i) {
        double _nLength = _vecScatterNodes[i].ptPoint.GetLength();
        EXPECT_NEAR(_nLength, _nRadius, Precision::RealTolerance());
        EXPECT_GE(_vecScatterNodes[i].ptDeriv1.GetLength(), Precision::RealTolerance());
        DPOINT3 _nxOuterProduct = CrossProduct(_vecScatterNodes[i].ptDeriv1, _vecScatterNodes[i].ptDeriv2);
        double _nCurvature = _nxOuterProduct.GetLength() / pow(_vecScatterNodes[i].ptDeriv1.GetLength(), 3);
        EXPECT_NEAR(_nCurvature, 1.0 / _nRadius, Precision::RealTolerance());
    }
}

TEST(NeatScatterUT, 2DEllipseNurbsNode)
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
    DPOINT2 _ptLeftFocus(-sqrt(pow(_nLongRadius, 2) - pow(_nShortRadius, 2)), 0.0);
    DPOINT2 _ptRightFocus(-_ptLeftFocus[axis::x], -_ptLeftFocus[axis::y]);
    DPOINT2 _ptStartPoint(_nLongRadius, 0.0);
    DPOINT2 _ptEndPoint(0.0, _nShortRadius);
    _NurbsNode.vecPoles.push_back(_ptStartPoint);
    _NurbsNode.vecPoles.push_back(DPOINT2(_nLongRadius, -_nShortRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nLongRadius, -_nShortRadius));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nLongRadius, 0.0));
    _NurbsNode.vecPoles.push_back(DPOINT2(-_nLongRadius, _nShortRadius));
    _NurbsNode.vecPoles.push_back(_ptEndPoint);
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(0.5);
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(sqrt(2.0) / 2.0);
    _NurbsNode.vecWeights.push_back(1.0);

    // Scatter nurbs node
    const double _nDeflection = 0.001;
    VECDPOINT2 _vecScatterPoints;
    EXPECT_NO_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterPoints));
    EXPECT_GT((int)_vecScatterPoints.size(), 1);
    CheckDPoint2Equal(_vecScatterPoints.front(), _ptStartPoint, Precision::RealTolerance());
    CheckDPoint2Equal(_vecScatterPoints.back(), _ptEndPoint, Precision::RealTolerance());
    for (size_t i = 0, _nSize = _vecScatterPoints.size(); i < _nSize; ++i) {
        double _nLength = (_vecScatterPoints[i] - _ptLeftFocus).GetLength()
            + (_vecScatterPoints[i] - _ptRightFocus).GetLength();
        EXPECT_NEAR(_nLength, 2.0 * _nLongRadius, _nDeflection);
    }

    std::vector<ScatterNode2D> _vecScatterNodes;
    EXPECT_NO_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterNodes));
    EXPECT_GT((int)_vecScatterNodes.size(), 1);
    EXPECT_NEAR(_vecScatterNodes.front().nKnot, _NurbsNode.vecKnots.front(), Precision::RealTolerance());
    EXPECT_NEAR(_vecScatterNodes.back().nKnot, _NurbsNode.vecKnots.back(), Precision::RealTolerance());
    CheckDPoint2Equal(_vecScatterNodes.front().ptPoint, _ptStartPoint, Precision::RealTolerance());
    CheckDPoint2Equal(_vecScatterNodes.back().ptPoint, _ptEndPoint, Precision::RealTolerance());

    double _nMinCurvature = _nShortRadius / (_nLongRadius * _nLongRadius) - Precision::RealTolerance();
    double _nMaxCurvature = _nLongRadius / (_nShortRadius * _nShortRadius) + Precision::RealTolerance();
    for (size_t i = 0, _nSize = _vecScatterNodes.size(); i < _nSize; ++i) {
        double _nLength = (_vecScatterNodes[i].ptPoint - _ptLeftFocus).GetLength()
            + (_vecScatterNodes[i].ptPoint - _ptRightFocus).GetLength();
        EXPECT_NEAR(_nLength, 2.0 * _nLongRadius, _nDeflection);
        EXPECT_GE(_vecScatterNodes[i].ptDeriv1.GetLength(), Precision::RealTolerance());
        double _nOuterProduct = CrossProduct(_vecScatterNodes[i].ptDeriv1, _vecScatterNodes[i].ptDeriv2);
        double _nCurvature = fabs(_nOuterProduct) / pow(_vecScatterNodes[i].ptDeriv1.GetLength(), 3);
        EXPECT_GE(_nCurvature, _nMinCurvature);
        EXPECT_LE(_nCurvature, _nMaxCurvature);
    }
}

TEST(NeatScatterUT, 3DEllipseNurbsNode)
{
    // Three-dimensional ellipse: a semi-ellipse on a two-dimensional plane,
    // and then obtained by rotating transformation around the X axis
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

    double _nLongRadius = 60.0;
    double _nShortRadius = 40.0;
    DPOINT3 _ptLeftFocus(-sqrt(pow(_nLongRadius, 2) - pow(_nShortRadius, 2)), 0.0, 0.0);
    DPOINT3 _ptRightFocus(-_ptLeftFocus[axis::x], -_ptLeftFocus[axis::y], -_ptLeftFocus[axis::z]);
    DPOINT3 _ptStartPoint(_nLongRadius, 0.0, 0.0);
    DPOINT3 _ptEndPoint(-_nLongRadius, 0.0, 0.0);
    _NurbsNode.vecPoles.push_back(_ptStartPoint);
    _NurbsNode.vecPoles.push_back(DPOINT3(_nLongRadius, 2.0 * _nShortRadius * _nCosAngle, -2.0 * _nShortRadius * _nSinAngle));
    _NurbsNode.vecPoles.push_back(DPOINT3(-_nLongRadius, 2.0 * _nShortRadius * _nCosAngle, -2.0 * _nShortRadius * _nSinAngle));
    _NurbsNode.vecPoles.push_back(_ptEndPoint);
    _NurbsNode.vecWeights.push_back(1.0);
    _NurbsNode.vecWeights.push_back(1.0 / 3.0);
    _NurbsNode.vecWeights.push_back(1.0 / 3.0);
    _NurbsNode.vecWeights.push_back(1.0);

    // Scatter nurbs node
    const double _nDeflection = 0.001;
    VECDPOINT3 _vecScatterPoints;
    EXPECT_NO_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterPoints));
    EXPECT_GT((int)_vecScatterPoints.size(), 1);
    CheckDPoint3Equal(_vecScatterPoints.front(), _ptStartPoint, Precision::RealTolerance());
    CheckDPoint3Equal(_vecScatterPoints.back(), _ptEndPoint, Precision::RealTolerance());
    for (size_t i = 0, _nSize = _vecScatterPoints.size(); i < _nSize; ++i) {
        double _nLength = (_vecScatterPoints[i] - _ptLeftFocus).GetLength()
            + (_vecScatterPoints[i] - _ptRightFocus).GetLength();
        EXPECT_NEAR(_nLength, 2.0 * _nLongRadius, _nDeflection);
    }

    std::vector<ScatterNode3D> _vecScatterNodes;
    EXPECT_NO_THROW(ScatterNurbs(&_NurbsNode, _nDeflection, _vecScatterNodes));
    EXPECT_GT((int)_vecScatterNodes.size(), 1);
    EXPECT_NEAR(_vecScatterNodes.front().nKnot, _NurbsNode.vecKnots.front(), Precision::RealTolerance());
    EXPECT_NEAR(_vecScatterNodes.back().nKnot, _NurbsNode.vecKnots.back(), Precision::RealTolerance());
    CheckDPoint3Equal(_vecScatterNodes.front().ptPoint, _ptStartPoint, Precision::RealTolerance());
    CheckDPoint3Equal(_vecScatterNodes.back().ptPoint, _ptEndPoint, Precision::RealTolerance());

    double _nMinCurvature = _nShortRadius / (_nLongRadius * _nLongRadius) - Precision::RealTolerance();
    double _nMaxCurvature = _nLongRadius / (_nShortRadius * _nShortRadius) + Precision::RealTolerance();
    for (size_t i = 0, _nSize = _vecScatterNodes.size(); i < _nSize; ++i) {
        double _nLength = (_vecScatterNodes[i].ptPoint - _ptLeftFocus).GetLength()
            + (_vecScatterNodes[i].ptPoint - _ptRightFocus).GetLength();
        EXPECT_NEAR(_nLength, 2.0 * _nLongRadius, _nDeflection);
        EXPECT_GE(_vecScatterNodes[i].ptDeriv1.GetLength(), Precision::RealTolerance());
        DPOINT3 _nxOuterProduct = CrossProduct(_vecScatterNodes[i].ptDeriv1, _vecScatterNodes[i].ptDeriv2);
        double _nCurvature = _nxOuterProduct.GetLength() / pow(_vecScatterNodes[i].ptDeriv1.GetLength(), 3);
        EXPECT_GE(_nCurvature, _nMinCurvature);
        EXPECT_LE(_nCurvature, _nMaxCurvature);
    }
}