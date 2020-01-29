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

TEST(NeatFastVectorUT, Constructor)
{
    // Initialize
    CFastVector<double> _vecTester;

    EXPECT_EQ(_vecTester.size(), 0);
    EXPECT_TRUE(_vecTester.empty());
    EXPECT_NO_THROW(_vecTester.clear());
}

TEST(NeatFastVectorUT, VectorSize)
{
    CFastVector<DPOINT2> _vecTester;
    _vecTester.push_back(DPOINT2());
    EXPECT_EQ(_vecTester.size(), 1);

    _vecTester.push_back(DPOINT2(0.0, 1.0));
    ASSERT_EQ(_vecTester.size(), 2);

    _vecTester.pop_front();
    _vecTester.pop_back();
    EXPECT_EQ(_vecTester.size(), 0);

    _vecTester.resize(10);
    EXPECT_EQ(_vecTester.size(), 10);

    _vecTester.clear();
    EXPECT_TRUE(_vecTester.size() == 0);

    _vecTester.resize(20, DPOINT2(1.0, 2.0));
    for (int i = 0; i < 20; ++i) {
        CheckDpoint2Equal(_vecTester[i], DPOINT2(1.0, 2.0), Precision::RealTolerance());
    }
}

TEST(NeatFastVectorUT, VisitElement)
{
    CFastVector<DPOINT3> _vecTester;
    for (int i = 0; i < 10; ++i) {
        _vecTester.push_back(DPOINT3(i, i * i, sqrt(double(i))));
    }

    ASSERT_EQ(_vecTester.size(), 10);
    EXPECT_EQ(_vecTester[4][axis::x], 4);
    EXPECT_EQ(_vecTester[4][axis::y], 16);
    EXPECT_EQ(_vecTester[4][axis::z], 2);

    for (int i = 0; i < 10; ++i) {
        _vecTester[i][axis::x] = i * i;
        _vecTester[i][axis::y] = i + 10;
        _vecTester[i][axis::z] = i - 2 * i;
    }

    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(_vecTester[i][axis::x], i * i);
        EXPECT_EQ(_vecTester[i][axis::y], i + 10);
        EXPECT_EQ(_vecTester[i][axis::z], -i);
    }
}

TEST(NeatFastVectorUT, ModifyElement)
{
    CFastVector<int> _vecTester;
    _vecTester.push_back(10);
    _vecTester.push_back(30);
    _vecTester.push_back(50);

    EXPECT_EQ(_vecTester.front(), 10);
    EXPECT_EQ(_vecTester.back(), 50);

    _vecTester.front() = 20;
    _vecTester.back() = 0;

    EXPECT_EQ(_vecTester[0], 20);
    EXPECT_EQ(_vecTester[1], 30);
    EXPECT_EQ(_vecTester[2], 0);

    _vecTester.pop_front();
    _vecTester.pop_back();
    ASSERT_TRUE(_vecTester.size() == 1);
    EXPECT_EQ(_vecTester.front(), _vecTester.back());
    EXPECT_EQ(_vecTester.front(), 30);
}

TEST(NeatFastVectorUT, CopyElement)
{
    CFastVector<IPOINT3> _vecTester;
    for (int i = 0; i < 100; ++i) {
        _vecTester.push_back(IPOINT3(i, i * i, i * i * i));
    }

    CFastVector<IPOINT3> _vecTesterEx;
    for (int i = 100; i < 1000; ++i) {
        _vecTesterEx.push_back(IPOINT3(i, i * i, i * i * i));
    }

    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(_vecTester[i][axis::x], i);
        EXPECT_EQ(_vecTester[i][axis::y], i * i);
        EXPECT_EQ(_vecTester[i][axis::z], i * i * i);
    }

    for (int i = 100; i < 1000; ++i) {
        EXPECT_EQ(_vecTesterEx[i - 100][axis::x], i);
        EXPECT_EQ(_vecTesterEx[i - 100][axis::y], i * i);
        EXPECT_EQ(_vecTesterEx[i - 100][axis::z], i * i * i);
    }

    _vecTester.copy(_vecTesterEx);
    ASSERT_EQ(_vecTester.size(), 1000);
    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(_vecTester[i][axis::x], i);
        EXPECT_EQ(_vecTester[i][axis::y], i * i);
        EXPECT_EQ(_vecTester[i][axis::z], i * i * i);
    }

    _vecTesterEx.clear();
    _vecTesterEx.copy(_vecTester);
    ASSERT_EQ(_vecTesterEx.size(), 1000);
    EXPECT_EQ(_vecTesterEx.size(), _vecTester.size());
    for (int i = 0; i < 1000; ++i) {
        EXPECT_EQ(_vecTesterEx[i][axis::x], _vecTester[i][axis::x]);
        EXPECT_EQ(_vecTesterEx[i][axis::x], i);
        EXPECT_EQ(_vecTesterEx[i][axis::y], _vecTester[i][axis::y]);
        EXPECT_EQ(_vecTesterEx[i][axis::y], i * i);
        EXPECT_EQ(_vecTesterEx[i][axis::z], _vecTester[i][axis::z]);
        EXPECT_EQ(_vecTesterEx[i][axis::z], i * i * i);
    }
}