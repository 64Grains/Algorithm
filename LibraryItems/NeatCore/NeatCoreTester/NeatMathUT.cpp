/***************************************************************************************************
 Created on: 2020-01-29
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatCore Library tester application.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#include "NeatCoreTester.h"

using namespace neat;

/**
 * This file unit tests the defined some simple mathematical function interfaces.
 */
TEST(NeatMathUT, Sign)
{
    EXPECT_EQ(Sign(0.0), 1.0);
    EXPECT_EQ(Sign(10.34), 1.0);
    EXPECT_EQ(Sign(-0.000001), -1.0);
}

TEST(NeatMathUT, DotProduct)
{
    DPOINT2 _ptPoint21(2.0, 3.0);
    DPOINT2 _ptPoint22(4.0, 5.0);
    EXPECT_NEAR(DotProduct(_ptPoint21, _ptPoint22), 23.0, Precision::RealTolerance());

    _ptPoint21[axis::x] = 2.4;
    _ptPoint21[axis::y] = 0.0;
    _ptPoint22[axis::x] = 0.0;
    _ptPoint22[axis::y] = 3.584;
    EXPECT_NEAR(DotProduct(_ptPoint21, _ptPoint22), 0.0, Precision::RealTolerance());

    DPOINT3 _ptPoint31(1.0, 2.0, 3.0);
    DPOINT3 _ptPoint32(7.0, 5.0, 3.0);
    EXPECT_NEAR(DotProduct(_ptPoint31, _ptPoint32), 26.0, Precision::RealTolerance());

    _ptPoint31[axis::x] = 1.0;
    _ptPoint31[axis::y] = 1.0;
    _ptPoint31[axis::z] = 1.0;
    _ptPoint32[axis::x] = 1.0;
    _ptPoint32[axis::y] = 0.0;
    _ptPoint32[axis::z] = -1.0;
    EXPECT_NEAR(DotProduct(_ptPoint31, _ptPoint32), 0.0, Precision::RealTolerance());
}

TEST(NeatMathUT, CrossProduct)
{
    DPOINT2 _ptPoint21(2.0, 3.0);
    DPOINT2 _ptPoint22(4.0, 5.0);
    EXPECT_NEAR(CrossProduct(_ptPoint21, _ptPoint22), -2.0, Precision::RealTolerance());

    _ptPoint21[axis::x] = 1.0;
    _ptPoint21[axis::y] = 3.0;
    _ptPoint22[axis::x] = -1.0;
    _ptPoint22[axis::y] = -3.0;
    EXPECT_NEAR(CrossProduct(_ptPoint21, _ptPoint22), 0.0, Precision::RealTolerance());

    DPOINT3 _ptPoint31(1.0, 0.0, 0.0);
    DPOINT3 _ptPoint32(0.0, 1.0, 0.0);
    CheckDpoint3Equal(CrossProduct(_ptPoint31, _ptPoint32), DPOINT3(0.0, 0.0, 1.0), Precision::RealTolerance());

    _ptPoint31[axis::x] = 1.0;
    _ptPoint31[axis::y] = 2.0;
    _ptPoint31[axis::z] = 3.0;
    _ptPoint32[axis::x] = 4.0;
    _ptPoint32[axis::y] = 5.0;
    _ptPoint32[axis::z] = 6.0;
    CheckDpoint3Equal(CrossProduct(_ptPoint31, _ptPoint32), DPOINT3(-3.0, 6.0, -3.0), Precision::RealTolerance());
}

TEST(NeatMathUT, ComputeAngle)
{
    EXPECT_NEAR(ComputeAngle(DPOINT2(1.0, 1.0)), Precision::PIE() / 4.0, Precision::RealTolerance());
    EXPECT_NEAR(ComputeAngle(DPOINT2(-1.0, 1.0)), Precision::PIE() * 3.0 / 4.0, Precision::RealTolerance());
    EXPECT_NEAR(ComputeAngle(DPOINT2(-1.0, -1.0)), Precision::PIE() * 5.0 / 4.0, Precision::RealTolerance());
    EXPECT_NEAR(ComputeAngle(DPOINT2(1.0, -1.0)), Precision::PIE() * 7.0 / 4.0, Precision::RealTolerance());

    EXPECT_NEAR(ComputeAngle(1.0, 0.0), 0.0, Precision::RealTolerance());
    EXPECT_NEAR(ComputeAngle(0.0, 1.0), Precision::PIE() / 2.0, Precision::RealTolerance());
    EXPECT_NEAR(ComputeAngle(-1.0, 0.0), Precision::PIE(), Precision::RealTolerance());
    EXPECT_NEAR(ComputeAngle(0.0, -1.0), Precision::PIE() * 3.0 / 2.0, Precision::RealTolerance());

    EXPECT_NEAR(ComputeAngle(DPOINT2(12345, 23.445)), ComputeAngle(12345, 23.445), Precision::RealTolerance());
    EXPECT_NEAR(ComputeAngle(DPOINT2(-1.2345, -23.445)), ComputeAngle(-1.2345, -23.445), Precision::RealTolerance());
    EXPECT_NEAR(ComputeAngle(DPOINT2(-1.2345, 23445)), ComputeAngle(-1.2345, 23445), Precision::RealTolerance());
    EXPECT_NEAR(ComputeAngle(DPOINT2(123.45, -2344.5)), ComputeAngle(123.45, -2344.5), Precision::RealTolerance());
}

/**
 * Check arc parameters
 */
static void CheckArcNode2D(const DPOINT2& ptStart_, const DPOINT2& ptEnd_, double nBulge_,
    const DPOINT2& ptCenter_, double nRadius_, double nStartAngle_, double nEndAngle_)
{
    ArcNode2D _ArcNode2D;
    if (Precision::IsAlmostEqual(nBulge_, 0.0, Precision::RealTolerance())) {
        EXPECT_THROW(RetrieveArcNode2D(ptStart_, ptEnd_, nBulge_, _ArcNode2D), std::exception);
    }
    else {
        ASSERT_NO_THROW(RetrieveArcNode2D(ptStart_, ptEnd_, nBulge_, _ArcNode2D));
        CheckDpoint2Equal(_ArcNode2D.ptCenter, ptCenter_, Precision::RealTolerance());
        EXPECT_NEAR(_ArcNode2D.nRadius, nRadius_, Precision::RealTolerance());
        EXPECT_NEAR(_ArcNode2D.nStartAngle, nStartAngle_, Precision::RealTolerance());
        EXPECT_NEAR(_ArcNode2D.nEndAngle, nEndAngle_, Precision::RealTolerance());
    }
}

/**
 * Construct arc parameters
 */
static void ConstructArcParameters(const DPOINT2& ptCenter_, double nRadius_, double nStartAngle_, double nEndAngle_,
    DPOINT2& ptStart_, DPOINT2& ptEnd_, double& nBulge_)
{
    ptStart_[axis::x] = ptCenter_[axis::x] + nRadius_ * cos(nStartAngle_);
    ptStart_[axis::y] = ptCenter_[axis::y] + nRadius_ * sin(nStartAngle_);
    ptEnd_[axis::x] = ptCenter_[axis::x] + nRadius_ * cos(nEndAngle_);
    ptEnd_[axis::y] = ptCenter_[axis::y] + nRadius_ * sin(nEndAngle_);
    nBulge_ = tan((nEndAngle_ - nStartAngle_) * 0.25);
}

TEST(NeatMathUT, RetrieveArcNode2D)
{
    DPOINT2 _ptStart(10.0, 20.0);
    DPOINT2 _ptEnd(20.0, 10.0);
    double _nBulge = 0.0;

    // Bulge is zero, throw exception
    CheckArcNode2D(_ptStart, _ptEnd, _nBulge, DPOINT2(0.0, 0.0), 0.0, 0.0, 0.0);

    // Zero-length arc
    _ptStart = DPOINT2(10.0, 20.0);
    _ptEnd = _ptStart;
    _nBulge = -1.0;
    CheckArcNode2D(_ptStart, _ptEnd, _nBulge, _ptStart, 0.0, 0.0, 0.0);

    // Clockwise minor arc
    DPOINT2 _ptCenter(exp(5.0), log(123456789.01));
    double _nRadius = pow(1.234, 3.4567);
    double _nStartAngle = Precision::PIE();
    double _nEndAngle = Precision::PIE() * 0.25;
    ConstructArcParameters(_ptCenter, _nRadius, _nStartAngle, _nEndAngle, _ptStart, _ptEnd, _nBulge);
    CheckArcNode2D(_ptStart, _ptEnd, _nBulge, _ptCenter, _nRadius, _nStartAngle, _nEndAngle);

    // Clockwise major arc
    _nStartAngle = Precision::PIE() * 1.8;
    _nEndAngle = Precision::PIE() * 0.6;
    ConstructArcParameters(_ptCenter, _nRadius, _nStartAngle, _nEndAngle, _ptStart, _ptEnd, _nBulge);
    CheckArcNode2D(_ptStart, _ptEnd, _nBulge, _ptCenter, _nRadius, _nStartAngle, _nEndAngle);

    // Counterclockwise minor arc
    _nStartAngle = Precision::PIE() * 1.3;
    _nEndAngle = Precision::PIE() * 1.4;
    ConstructArcParameters(_ptCenter, _nRadius, _nStartAngle, _nEndAngle, _ptStart, _ptEnd, _nBulge);
    CheckArcNode2D(_ptStart, _ptEnd, _nBulge, _ptCenter, _nRadius, _nStartAngle, _nEndAngle);
 
    // Counterclockwise major arc
    _nStartAngle = Precision::PIE() * 0.4;
    _nEndAngle = Precision::PIE() * 1.9;
    ConstructArcParameters(_ptCenter, _nRadius, _nStartAngle, _nEndAngle, _ptStart, _ptEnd, _nBulge);
    CheckArcNode2D(_ptStart, _ptEnd, _nBulge, _ptCenter, _nRadius, _nStartAngle, _nEndAngle);

    // Clockwise arc, and cross the first and fourth quadrants
    _nStartAngle = Precision::PIE() * 0.001;
    _nEndAngle = Precision::PIE() * -0.001;
    ConstructArcParameters(_ptCenter, _nRadius, _nStartAngle, _nEndAngle, _ptStart, _ptEnd, _nBulge);
    CheckArcNode2D(_ptStart, _ptEnd, _nBulge, _ptCenter, _nRadius, _nStartAngle, _nEndAngle);

    // Counterclockwise arc, and cross the first and fourth quadrants
    _nStartAngle = Precision::PIE() * 1.9995;
    _nEndAngle = Precision::PIE() * 2.00001;
    ConstructArcParameters(_ptCenter, _nRadius, _nStartAngle, _nEndAngle, _ptStart, _ptEnd, _nBulge);
    CheckArcNode2D(_ptStart, _ptEnd, _nBulge, _ptCenter, _nRadius, _nStartAngle, _nEndAngle);
}