/***************************************************************************************************
 Created on: 2020-01-28
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
 * This file unit tests defined macros.
 */
TEST(NeatMacroUT, max)
{
    EXPECT_EQ(max(1, 2), 2);
    EXPECT_EQ(max(-0.5, -5.6), -0.5);
    EXPECT_EQ(max(1.6, -0.3), 1.6);
}

TEST(NeatMacroUT, min)
{
    EXPECT_EQ(0, min(0, 2));
    EXPECT_EQ(-4.0, min(0, -4.0));
    EXPECT_EQ(-2.5, min(2.5, -2.5));
}

TEST(NeatMacroUT, max3)
{
    EXPECT_EQ(max3(-2, 0, 2), 2);
    EXPECT_EQ(max3(1.6, 3.2, 1.6), 3.2);
    EXPECT_EQ(max3(0, -5.0, -6.4), 0);
}

TEST(NeatMacroUT, min3)
{
    EXPECT_EQ(0, min3(0, 0, 3));
    EXPECT_EQ(-4.5, min3(3.4, -4.5, 0));
    EXPECT_EQ(-6.4, min3(-2.3, -4.8, -6.4));
}

TEST(NeatMacroUT, CUT)
{
    double _nLower = -2.5;
    double _nUpper = 3.7;

    double _szInit[] = { 0.0, -3.0, 5.0 };
    double _szAim[] = { 0.0, _nLower, _nUpper };

    for (int i = 0; i < 3; ++i) {
        double x = _szInit[i];
        __CUT(x, _nLower, _nUpper);
        EXPECT_EQ(x, _szAim[i]);
    }
}

TEST(NeatMacroUT, exception)
{
#ifndef NO_EXCEPTION_FOR_NEAT
    EXPECT_THROW(NEAT_RAISE_IF(true, "An exception occurs"), std::exception);
    EXPECT_NO_THROW(NEAT_RAISE_IF(false, "An exception occurs"));
#else
#pragma warning(disable:4003)
    EXPECT_NO_THROW(NEAT_RAISE_IF(true, "An exception occurs"));
    EXPECT_NO_THROW(NEAT_RAISE_IF(false, "An exception occurs"));
#pragma warning(default:4003)
#endif

    EXPECT_THROW(NEAT_RAISE(true, "An exception occurs"), std::exception);
    EXPECT_NO_THROW(NEAT_RAISE(false, "An exception occurs"));
}