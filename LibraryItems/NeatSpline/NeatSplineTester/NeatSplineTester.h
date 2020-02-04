/***************************************************************************************************
 Created on: 2020-02-01
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library tester application.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_SPLINE_TESTER_H_20200201__
#define __NEAT_SPLINE_TESTER_H_20200201__

#include <list>

/**
 * Check if two-dimensional space coordinates are equal
 */
void CheckDPoint2Equal(const neat::DPOINT2& ptLeft_, const neat::DPOINT2& ptRight_, double nTolerance_);

/**
 * Check if three-dimensional space coordinates are equal
 */
void CheckDPoint3Equal(const neat::DPOINT3& ptLeft_, const neat::DPOINT3& ptRight_, double nTolerance_);

/**
 * Read nurbs node from file
 */
bool ReadNurbsFromFile(const std::string& strFilePath_, std::vector<neat::NurbsNode3D>& vecNurbsNodes_);

/**
 * Write points to file
 */
bool WritePointsToFile(const std::string& strFilePath_, const neat::VECDPOINT3& vecPoints_);

/**
 * Write polyline to file
 */
bool WritePolylineToFile(const std::string& strFilePath_, const neat::Polyline2D& Polyline2D_);

/**
 * Test data information
 */
struct NurbsFileInfo
{
    std::string strFileDir;
    std::string strFileName;
};

class CNurbsFileInfoList : public std::list<NurbsFileInfo>
{
public:
    CNurbsFileInfoList();
};

#endif // __NEAT_SPLINE_TESTER_H_20200201__