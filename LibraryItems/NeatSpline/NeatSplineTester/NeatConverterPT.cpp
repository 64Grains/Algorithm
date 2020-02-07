/***************************************************************************************************
 Created on: 2020-02-01
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library tester application.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#include "NeatSplineTester.h"

using namespace neat;
using namespace tester;

/**
 * Read the converted bezier splines from file.
 */
static void ReadConvertResult(const std::string& strFilePath_, std::vector<BezierNode3D>& vecConvertResult_);
static void CheckConverterTest(const std::vector<BezierNode2D>& vecLeft_, const std::vector<BezierNode2D>& vecRight_);
static void CheckConverterTest(const std::vector<BezierNode3D>& vecLeft_, const std::vector<BezierNode3D>& vecRight_);
static bool IsRationalSpline(const VECDOUBLE& vecWeights_);

/**
 * This file parameter tests the interface for converting splines.
 */
const static CTestFileInfoList gs_listTestFileInfo;

class CNurbsConverterTest : public ::testing::TestWithParam<TestFileInfo>
{
};

TEST_P(CNurbsConverterTest, NurbsFiles)
{
    const TestFileInfo& _fileInfo = GetParam();

    // Read verification result
    std::vector<BezierNode3D> _vecConvertResult;
    ReadConvertResult(_fileInfo.strFileDir + _fileInfo.strConverterFile, _vecConvertResult);

    // Read test file
    std::vector<NurbsNode3D> _vecNurbsNodes;
    ReadNurbsFromFile(_fileInfo.strFileDir + _fileInfo.strFileName, _vecNurbsNodes);

    // Check if it can downgrade
    std::vector<BezierNode2D> _vecConvertResult2D;
    std::vector<NurbsNode2D> _vecNurbsNodes2D;
    if (DowngradeBezierNode(_vecConvertResult, _vecConvertResult2D) && DowngradeNurbsNode(_vecNurbsNodes, _vecNurbsNodes2D)) {
        std::vector<BezierNode2D> _vecBezierNodes;
        for (const auto& _NurbsNode : _vecNurbsNodes2D) {
            std::vector<BezierNode2D> _vecTempBezierNodes;
            ConvertNurbsToBezier(&_NurbsNode, _vecTempBezierNodes);

            for (const auto& _BezierNode : _vecTempBezierNodes) {
                _vecBezierNodes.emplace_back(_BezierNode);
            }
        }

        // Check conversion results
        CheckConverterTest(_vecConvertResult2D, _vecBezierNodes);
    }
    else {
        std::vector<BezierNode3D> _vecBezierNodes;
        for (const auto& _NurbsNode : _vecNurbsNodes) {
            std::vector<BezierNode3D> _vecTempBezierNodes;
            ConvertNurbsToBezier(&_NurbsNode, _vecTempBezierNodes);

            for (const auto& _BezierNode : _vecTempBezierNodes) {
                _vecBezierNodes.emplace_back(_BezierNode);
            }
        }

        // Check conversion results
        CheckConverterTest(_vecConvertResult, _vecBezierNodes);
    }
}

INSTANTIATE_TEST_CASE_P(NeatConverterPT, CNurbsConverterTest, testing::ValuesIn(gs_listTestFileInfo));

//////////////////////////////////////////////////////////////////////////
// local function
static void ReadConvertResult(const std::string& strFilePath_, std::vector<BezierNode3D>& vecConvertResult_)
{
    std::vector<NurbsNode3D> _vecNurbsNodes;
    ReadNurbsFromFile(strFilePath_, _vecNurbsNodes);

    vecConvertResult_.clear();
    for (const auto& _NurbsNode : _vecNurbsNodes) {
        BezierNode3D _BezierNode;
        ASSERT_TRUE(TransformNurbsNode(_NurbsNode, _BezierNode));
        vecConvertResult_.emplace_back(_BezierNode);
    }
}

static void CheckConverterTest(const std::vector<BezierNode2D>& vecLeft_, const std::vector<BezierNode2D>& vecRight_)
{
    ASSERT_EQ(vecLeft_.size(), vecRight_.size());
    for (size_t i = 0; i < vecLeft_.size(); ++i) {
        EXPECT_NEAR(vecLeft_[i].nStartKnot, vecRight_[i].nStartKnot, Precision::RealTolerance());
        EXPECT_NEAR(vecLeft_[i].nEndKnot, vecRight_[i].nEndKnot, Precision::RealTolerance());

        ASSERT_EQ(vecLeft_[i].vecPoles.size(), vecRight_[i].vecPoles.size());
        for (size_t j = 0; j < vecLeft_[i].vecPoles.size(); ++j) {
            CheckDPoint2Equal(vecLeft_[i].vecPoles[j], vecRight_[i].vecPoles[j], Precision::RealTolerance());
        }

        bool _bLeftRational = IsRationalSpline(vecLeft_[i].vecWeights);
        ASSERT_EQ(_bLeftRational, IsRationalSpline(vecRight_[i].vecWeights));
        if (_bLeftRational) {
            ASSERT_EQ(vecLeft_[i].vecWeights.size(), vecRight_[i].vecWeights.size());
            for (size_t j = 0; j < vecLeft_[i].vecWeights.size(); ++j) {
                EXPECT_NEAR(vecLeft_[i].vecWeights[j], vecRight_[i].vecWeights[j], Precision::RealTolerance());
            }
        }
    }
}

static void CheckConverterTest(const std::vector<BezierNode3D>& vecLeft_, const std::vector<BezierNode3D>& vecRight_)
{
    ASSERT_EQ(vecLeft_.size(), vecRight_.size());
    for (size_t i = 0; i < vecLeft_.size(); ++i) {
        EXPECT_NEAR(vecLeft_[i].nStartKnot, vecRight_[i].nStartKnot, Precision::RealTolerance());
        EXPECT_NEAR(vecLeft_[i].nEndKnot, vecRight_[i].nEndKnot, Precision::RealTolerance());

        ASSERT_EQ(vecLeft_[i].vecPoles.size(), vecRight_[i].vecPoles.size());
        for (size_t j = 0; j < vecLeft_[i].vecPoles.size(); ++j) {
            CheckDPoint3Equal(vecLeft_[i].vecPoles[j], vecRight_[i].vecPoles[j], Precision::RealTolerance());
        }

        bool _bLeftRational = IsRationalSpline(vecLeft_[i].vecWeights);
        ASSERT_EQ(_bLeftRational, IsRationalSpline(vecRight_[i].vecWeights));
        if (_bLeftRational) {
            ASSERT_EQ(vecLeft_[i].vecWeights.size(), vecRight_[i].vecWeights.size());
            for (size_t j = 0; j < vecLeft_[i].vecWeights.size(); ++j) {
                EXPECT_NEAR(vecLeft_[i].vecWeights[j], vecRight_[i].vecWeights[j], Precision::RealTolerance());
            }
        }
    }
}

static bool IsRationalSpline(const VECDOUBLE& vecWeights_)
{
    if (vecWeights_.empty()) {
        return false;
    }

    double _nWeight = vecWeights_.front();
    for (size_t i = 1; i < vecWeights_.size(); ++i) {
        if (fabs(vecWeights_[i] - _nWeight) > Precision::RealTolerance()) {
            return true;
        }
    }

    return false;
}