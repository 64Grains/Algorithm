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