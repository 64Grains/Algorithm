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
 * This file unit tests the defined coordinate points in the Cartesian coordinate system.
 */
TEST(NeatPointUT, axis)
{
    EXPECT_EQ(axis::x, 0);
    EXPECT_EQ(axis::y, 1);
    EXPECT_EQ(axis::z, 2);
}

TEST(NeatPointUT, DPOINT2)
{
    DPOINT2 _ptPoint1;  _ptPoint1.SetSame(2.0);
    EXPECT_NEAR(_ptPoint1[axis::x], 2.0, Precision::RealTolerance());
    EXPECT_NEAR(_ptPoint1[axis::y], 2.0, Precision::RealTolerance());

    DPOINT2 _ptTemp = _ptPoint1;
    EXPECT_NEAR(_ptTemp[axis::x], 2.0, Precision::RealTolerance());
    EXPECT_NEAR(_ptTemp[axis::y], 2.0, Precision::RealTolerance());

    DPOINT2 _ptPoint2(5.4, 3.8);
    _ptTemp += _ptPoint2;
    EXPECT_NEAR(_ptTemp[axis::x], 7.4, Precision::RealTolerance());
    EXPECT_NEAR(_ptTemp[axis::y], 5.8, Precision::RealTolerance());

    _ptTemp = _ptPoint1;
    _ptTemp -= _ptPoint2;
    EXPECT_NEAR(_ptTemp[axis::x], -3.4, Precision::RealTolerance());
    EXPECT_NEAR(_ptTemp[axis::y], -1.8, Precision::RealTolerance());

    _ptTemp = _ptPoint1 * 2.0;
    EXPECT_NEAR(_ptTemp[axis::x], 4.0, Precision::RealTolerance());
    EXPECT_NEAR(_ptTemp[axis::y], 4.0, Precision::RealTolerance());

    _ptTemp = _ptPoint2 / 2.0;
    EXPECT_NEAR(_ptTemp[axis::x], 2.7, Precision::RealTolerance());
    EXPECT_NEAR(_ptTemp[axis::y], 1.9, Precision::RealTolerance());

    EXPECT_NEAR(_ptPoint1.GetLength(), 2.0 * sqrt(2.0), Precision::RealTolerance());
    EXPECT_NEAR(_ptPoint2.GetSquareLength(), 43.6, Precision::RealTolerance());
}

TEST(NeatPointUT, DPOINT3)
{
    DPOINT3 _ptPoint1(10.0, 20.0, 30.0);
    DPOINT3 _ptPoint2;
    _ptPoint2[axis::x] = 50.0;
    _ptPoint2[axis::y] = 50.0;
    _ptPoint2[axis::z] = 50.0;

    DPOINT3 _ptTemp = _ptPoint1 + _ptPoint2;
    EXPECT_NEAR(_ptTemp[axis::x], 60.0, Precision::RealTolerance());
    EXPECT_NEAR(_ptTemp[axis::y], 70.0, Precision::RealTolerance());
    EXPECT_NEAR(_ptTemp[axis::z], 80.0, Precision::RealTolerance());

    _ptTemp = _ptPoint1;
    _ptTemp *= 10.0;
    EXPECT_NEAR(_ptTemp[axis::x], 100.0, Precision::RealTolerance());
    EXPECT_NEAR(_ptTemp[axis::y], 200.0, Precision::RealTolerance());
    EXPECT_NEAR(_ptTemp[axis::z], 300.0, Precision::RealTolerance());

    _ptTemp = _ptPoint2;
    _ptTemp /= 20.0;
    EXPECT_NEAR(_ptTemp[axis::x], 2.5, Precision::RealTolerance());
    EXPECT_NEAR(_ptTemp[axis::y], 2.5, Precision::RealTolerance());
    EXPECT_NEAR(_ptTemp[axis::z], 2.5, Precision::RealTolerance());

    EXPECT_NEAR(_ptPoint1.GetLength(), 10.0 * sqrt(14.0), Precision::RealTolerance());
    EXPECT_NEAR(_ptPoint2.GetSquareLength(), 7500.0, Precision::RealTolerance());
}

TEST(NeatPointUT, DPOINT4)
{
    DPOINT4 _ptPoint1;  _ptPoint1.SetSame(5.0);
    DPOINT4 _ptPoint2;  _ptPoint2.SetSame(30.0);

    DPOINT4 _ptTemp = _ptPoint1 * 2.0 + 3.0 * _ptPoint2;
    for (int i = 0; i < 4; ++i) {
        EXPECT_NEAR(_ptTemp[i], 100.0, Precision::RealTolerance());
    }

    _ptTemp = _ptPoint1 - _ptPoint2 / 10.0;
    for (int i = 0; i < 4; ++i) {
        EXPECT_NEAR(_ptTemp[i], 2.0, Precision::RealTolerance());
    }
}

TEST(NeatPointUT, DPOINT5)
{
    DPOINT5 _ptPoint1; _ptPoint1.SetSame(10.32);
    DPOINT5 _ptPoint2; _ptPoint2.SetSame(20.01);

    DPOINT5 _ptTemp = _ptPoint1;
    _ptTemp += _ptPoint2;
    for (int i = 0; i < 5; ++i) {
        EXPECT_NEAR(_ptTemp[i], 30.33, Precision::RealTolerance());
    }

    _ptTemp = _ptPoint1;
    _ptTemp -= _ptPoint2;
    for (int i = 0; i < 5; ++i) {
        EXPECT_NEAR(_ptTemp[i], -9.69, Precision::RealTolerance());
    }

    _ptTemp = _ptPoint1;
    _ptTemp *= 8.6;
    for (int i = 0; i < 5; ++i) {
        EXPECT_NEAR(_ptTemp[i], 88.752, Precision::RealTolerance());
    }

    _ptTemp = _ptPoint2;
    _ptTemp /= 5.0;
    for (int i = 0; i < 5; ++i) {
        EXPECT_NEAR(_ptTemp[i], 4.002, Precision::RealTolerance());
    }
}

TEST(NeatPointUT, DPOINT6)
{
    DPOINT6 _ptPoint1;  _ptPoint1.SetSame(5.4);
    DPOINT6 _ptPoint2;  _ptPoint2.SetSame(1.7);

    DPOINT6 _ptTemp = _ptPoint1 + _ptPoint2;
    for (int i = 0; i < 6; ++i) {
        EXPECT_NEAR(_ptTemp[i], 7.1, Precision::RealTolerance());
    }

    _ptTemp = _ptPoint1 - _ptPoint2;
    for (int i = 0; i < 6; ++i) {
        EXPECT_NEAR(_ptTemp[i], 3.7, Precision::RealTolerance());
    }

    _ptTemp = _ptPoint1 * 3.3;
    for (int i = 0; i < 6; ++i) {
        EXPECT_NEAR(_ptTemp[i], 17.82, Precision::RealTolerance());
    }

    _ptTemp = 7.8 * _ptPoint2;
    for (int i = 0; i < 6; ++i) {
        EXPECT_NEAR(_ptTemp[i], 13.26, Precision::RealTolerance());
    }

    _ptTemp = _ptPoint1 / 5.0;
    for (int i = 0; i < 6; ++i) {
        EXPECT_NEAR(_ptTemp[i], 1.08, Precision::RealTolerance());
    }
}

TEST(NeatPointUT, IPOINT2)
{
    IPOINT2 _ptPoint1;  _ptPoint1.SetSame(2);
    EXPECT_EQ(_ptPoint1[axis::x], 2);
    EXPECT_EQ(_ptPoint1[axis::y], 2);

    IPOINT2 _ptTemp = _ptPoint1;
    EXPECT_EQ(_ptTemp[axis::x], 2);
    EXPECT_EQ(_ptTemp[axis::y], 2);

    IPOINT2 _ptPoint2(5, 3);
    _ptTemp += _ptPoint2;
    EXPECT_EQ(_ptTemp[axis::x], 7);
    EXPECT_EQ(_ptTemp[axis::y], 5);

    _ptTemp = _ptPoint1;
    _ptTemp -= _ptPoint2;
    EXPECT_EQ(_ptTemp[axis::x], -3);
    EXPECT_EQ(_ptTemp[axis::y], -1);

    _ptTemp = _ptPoint1 * 2;
    EXPECT_EQ(_ptTemp[axis::x], 4);
    EXPECT_EQ(_ptTemp[axis::y], 4);

    _ptTemp = _ptPoint2 / 2;
    EXPECT_EQ(_ptTemp[axis::x], 2);
    EXPECT_EQ(_ptTemp[axis::y], 1);
}

TEST(NeatPointUT, IPOINT3)
{
    IPOINT3 _ptPoint1(5, 4, 3);
    IPOINT3 _ptPoint2;
    _ptPoint2[axis::x] = 30;
    _ptPoint2[axis::y] = 40;
    _ptPoint2[axis::z] = 50;

    IPOINT3 _ptTemp = _ptPoint1 + _ptPoint2;
    EXPECT_EQ(_ptTemp[axis::x], 35);
    EXPECT_EQ(_ptTemp[axis::y], 44);
    EXPECT_EQ(_ptTemp[axis::z], 53);

    _ptTemp = _ptPoint1;
    _ptTemp *= 10;
    EXPECT_EQ(_ptTemp[axis::x], 50);
    EXPECT_EQ(_ptTemp[axis::y], 40);
    EXPECT_EQ(_ptTemp[axis::z], 30);

    _ptTemp = _ptPoint2;
    _ptTemp /= 20;
    EXPECT_EQ(_ptTemp[axis::x], 1);
    EXPECT_EQ(_ptTemp[axis::y], 2);
    EXPECT_EQ(_ptTemp[axis::z], 2);

    _ptTemp = _ptPoint1 * 2 + 3 * _ptPoint2;
    EXPECT_EQ(_ptTemp[axis::x], 100);
    EXPECT_EQ(_ptTemp[axis::y], 128);
    EXPECT_EQ(_ptTemp[axis::z], 156);

    _ptTemp = _ptPoint1 - _ptPoint2 / 10;
    EXPECT_EQ(_ptTemp[axis::x], 2);
    EXPECT_EQ(_ptTemp[axis::y], 0);
    EXPECT_EQ(_ptTemp[axis::z], -2);
}

TEST(NeatPointUT, BPOINT2)
{
    BPOINT2 _ptPoint1;  _ptPoint1.SetSame(true);
    EXPECT_EQ(_ptPoint1[axis::x], true);
    EXPECT_EQ(_ptPoint1[axis::y], true);

    BPOINT2 _ptTemp = _ptPoint1;
    EXPECT_EQ(_ptTemp[axis::x], true);
    EXPECT_EQ(_ptTemp[axis::y], true);

    BPOINT2 _ptPoint2(true, false);
    _ptTemp[axis::x] = _ptPoint2[axis::x];
    _ptTemp[axis::y] = _ptPoint2[axis::y];
    EXPECT_EQ(_ptTemp[axis::x], true);
    EXPECT_EQ(_ptTemp[axis::y], false);
}

TEST(NeatPointUT, BPOINT3)
{
    BPOINT3 _ptPoint1(true, false, false);
    BPOINT3 _ptPoint2;
    _ptPoint2[axis::x] = false;
    _ptPoint2[axis::y] = true;
    _ptPoint2[axis::z] = true;

    {
        // Compile error
        // BPOINT3 _ptTemp = _ptPoint1 + _ptPoint2;
    }

    BPOINT3 _ptTemp = _ptPoint1;
    EXPECT_EQ(_ptTemp[axis::x], true);
    EXPECT_EQ(_ptTemp[axis::y], false);
    EXPECT_EQ(_ptTemp[axis::z], false);

    _ptTemp[axis::x] = _ptPoint2[axis::x];
    _ptTemp[axis::y] = _ptPoint2[axis::y];
    _ptTemp[axis::z] = _ptPoint2[axis::z];
    EXPECT_EQ(_ptTemp[axis::x], false);
    EXPECT_EQ(_ptTemp[axis::y], true);
    EXPECT_EQ(_ptTemp[axis::z], true);
}