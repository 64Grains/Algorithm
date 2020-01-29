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
 * This file contains the 'main' function. Program execution begins and ends there.
 */
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/**
 * Global test function.
 */
void CheckDpoint2Equal(const neat::DPOINT2& ptLeft_, const neat::DPOINT2& ptRight_, double nTolerance_)
{
    EXPECT_NEAR(ptLeft_[axis::x], ptRight_[axis::x], nTolerance_);
    EXPECT_NEAR(ptLeft_[axis::y], ptRight_[axis::y], nTolerance_);
}

void CheckDpoint3Equal(const neat::DPOINT3& ptLeft_, const neat::DPOINT3& ptRight_, double nTolerance_)
{
    EXPECT_NEAR(ptLeft_[axis::x], ptRight_[axis::x], nTolerance_);
    EXPECT_NEAR(ptLeft_[axis::y], ptRight_[axis::y], nTolerance_);
    EXPECT_NEAR(ptLeft_[axis::z], ptRight_[axis::z], nTolerance_);
}