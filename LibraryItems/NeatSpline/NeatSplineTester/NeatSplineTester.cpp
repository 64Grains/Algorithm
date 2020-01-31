/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library tester application.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
/**
 * This file contains the 'main' function. Program execution begins and ends there.
 */
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(NeatScatterUT, Bezier)
{
    neat::BezierNode2D _BezierNode2D;
    _BezierNode2D.vecPoles.push_back(neat::DPOINT2(10.0, 0.0));
    _BezierNode2D.vecPoles.push_back(neat::DPOINT2(10.0, 10.0));
    _BezierNode2D.vecPoles.push_back(neat::DPOINT2(-10.0, 10.0));
    _BezierNode2D.vecPoles.push_back(neat::DPOINT2(-10.0, 0.0));
    _BezierNode2D.vecWeights.push_back(1.0);
    _BezierNode2D.vecWeights.push_back(1.0 / 3.0);
    _BezierNode2D.vecWeights.push_back(1.0 / 3.0);
    _BezierNode2D.vecWeights.push_back(1.0);
    _BezierNode2D.nStartKnot = 0.0;
    _BezierNode2D.nEndKnot = 1.0;

    double _nDeflection = 0.001;
    neat::VECDPOINT2 _vecScatterPoints;
    ASSERT_NO_THROW(ScatterBezier(&_BezierNode2D, _nDeflection, _vecScatterPoints));
}