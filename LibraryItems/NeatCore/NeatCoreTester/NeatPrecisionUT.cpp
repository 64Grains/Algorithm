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
 * This file unit tests the defined precision values.
 */
TEST(NeatPrecisionUT, IsAlmostEqual)
{
    EXPECT_TRUE(Precision::IsAlmostEqual(0.0, 0.0, 0.0));
    EXPECT_TRUE(Precision::IsAlmostEqual(1.0, 1.0001, 0.0001));
    EXPECT_TRUE(Precision::IsAlmostEqual(1.0001, 1.0, 0.0001));
    EXPECT_TRUE(Precision::IsAlmostEqual(-1.0, -1.0001, 0.0001));
    EXPECT_TRUE(Precision::IsAlmostEqual(-1.0001, -1.0, 0.0001));

    EXPECT_TRUE(Precision::IsAlmostEqual(1.0001, 1.0, 0.001));
    EXPECT_FALSE(Precision::IsAlmostEqual(1.0001, 1.0, 0.00001));
}

TEST(NeatPrecisionUT, PIE)
{
    EXPECT_TRUE(Precision::IsAlmostEqual(cos(Precision::PIE()), -1.0, Precision::RealTolerance()));
    EXPECT_TRUE(Precision::IsAlmostEqual(sin(Precision::PIE()), 0.0, Precision::RealTolerance()));

    EXPECT_TRUE(Precision::IsAlmostEqual(cos(Precision::PIE() / 2), 0.0, Precision::RealTolerance()));
    EXPECT_TRUE(Precision::IsAlmostEqual(sin(Precision::PIE() / 2), 1.0, Precision::RealTolerance()));
}

TEST(NeatPrecisionUT, InvalidDouble)
{
    double _nValue1 = Precision::InvalidDouble();
    double _nValue2 = 2.0;

    EXPECT_TRUE(Precision::IsInvalidDouble(_nValue1));
    EXPECT_FALSE(Precision::IsInvalidDouble(_nValue2));
}

TEST(NeatPrecisionUT, InfinityDouble)
{
    double _nValue1 = Precision::InfinityDouble();
    double _nValue2 = 2.0;
    double _nValue3 = 1.0 / sin(0.0);

    EXPECT_TRUE(Precision::IsInfinityDouble(_nValue1));
    EXPECT_FALSE(Precision::IsInfinityDouble(_nValue2));
    EXPECT_TRUE(Precision::IsInfinityDouble(_nValue3));
}