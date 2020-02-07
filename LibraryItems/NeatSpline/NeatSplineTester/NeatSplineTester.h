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

namespace tester
{
    /**
     * Test case information
     */
    struct TestFileInfo
    {
        std::string strFileDir;
        std::string strFileName;
        std::string strParserFile;
        std::string strScatterFile;
        std::string strConverterFile;
    };

    class CTestFileInfoList : public std::list<TestFileInfo>
    {
    public:
        CTestFileInfoList();
    };

    /**
     * Check if space coordinates are equal
     */
    void CheckDPoint2Equal(const neat::DPOINT2& ptLeft_, const neat::DPOINT2& ptRight_, double nTolerance_);
    void CheckDPoint3Equal(const neat::DPOINT3& ptLeft_, const neat::DPOINT3& ptRight_, double nTolerance_);

    /**
     * Data dimension down grade
     */
    bool DowngradeDPoint(const neat::VECDPOINT3& vecPoint3D_, neat::VECDPOINT2& vecPoint2D_);
    bool DowngradeNurbsNode(const std::vector<neat::NurbsNode3D>& vecNurbs3D_, std::vector<neat::NurbsNode2D>& vecNurbs2D_);
    bool DowngradeBezierNode(const std::vector<neat::BezierNode3D>& vecBezier3D_, std::vector<neat::BezierNode2D>& vecBezier2D_);
    bool DowngradeScatterNode(const std::vector<neat::ScatterNode3D>& vecScatter3D_, std::vector<neat::ScatterNode2D>& vecScatter2D_);

    /**
     * Data form transformation
     */
    bool TransformNurbsNode(const neat::NurbsNode2D& NurbsNode_, neat::BezierNode2D& BezierNode_);
    bool TransformNurbsNode(const neat::NurbsNode3D& NurbsNode_, neat::BezierNode3D& BezierNode_);

    /**
     * Read nurbs node from file
     */
    bool ReadNurbsFromFile(const std::string& strFilePath_, std::vector<neat::NurbsNode3D>& vecNurbsNodes_);

    /**
     * Write polyline to file
     */
    bool WritePointsToFile(const std::string& strFilePath_, const neat::VECDPOINT3& vecPoints_);
    bool WritePointsToFile(const std::string& strFilePath_, const neat::VECDPOINT2& vecPoints_);
    bool WritePolylineToFile(const std::string& strFilePath_, const neat::Polyline2D& Polyline2D_);

} // End namespace tester

#endif // __NEAT_SPLINE_TESTER_H_20200201__