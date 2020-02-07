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
#include <fstream>
#include <sstream>

using namespace neat;
using namespace tester;

/**
 * Read the target knot and its corresponding point coordinates, first-order and second-order derivatives from file.
 */
static void ReadParserResult(const std::string& strFilePath_, std::vector<ScatterNode3D>& vecParserResult_);

/**
 * This file parameter tests the interface for parsing splines.
 */
const static CTestFileInfoList gs_listTestFileInfo;

class CNurbsParserTest : public ::testing::TestWithParam<TestFileInfo>
{
};

TEST_P(CNurbsParserTest, NurbsFiles)
{
    const TestFileInfo& _fileInfo = GetParam();

    // Read verification result
    std::vector<ScatterNode3D> _vecParserResult;
    ReadParserResult(_fileInfo.strFileDir + _fileInfo.strParserFile, _vecParserResult);

    // Read test file
    std::vector<NurbsNode3D> _vecNurbsNodes;
    ReadNurbsFromFile(_fileInfo.strFileDir + _fileInfo.strFileName, _vecNurbsNodes);

    double _nCheckError = 1.0e-7;

    // Check if it can downgrade
    std::vector<ScatterNode2D> _vecParserResult2D;
    std::vector<NurbsNode2D> _vecNurbsNodes2D;
    if (DowngradeScatterNode(_vecParserResult, _vecParserResult2D) && DowngradeNurbsNode(_vecNurbsNodes, _vecNurbsNodes2D)) {
        CNurbsParser2D _NurbsParser;
        CBezierParser2D _BezierParser;
        BezierNode2D _BezierNode;
        size_t _nNurbsIndex = 0;
        for (const auto& _ScatterNode : _vecParserResult2D) {
            size_t _nIndex = _nNurbsIndex;
            ASSERT_TRUE(_nIndex < _vecNurbsNodes2D.size());
            ASSERT_TRUE(_ScatterNode.nKnot > _vecNurbsNodes2D[_nIndex].vecKnots.front() - Precision::RealTolerance());
            while (_ScatterNode.nKnot > _vecNurbsNodes2D[_nIndex].vecKnots.back() + Precision::RealTolerance()) {
                ++_nIndex;
                ASSERT_TRUE(_nIndex < _vecNurbsNodes2D.size());
            }

            bool _bIsBezierNode = false;
            if (_nIndex == 0 || _nIndex != _nNurbsIndex) {
                _nNurbsIndex = _nIndex;
                _bIsBezierNode = TransformNurbsNode(_vecNurbsNodes2D[_nNurbsIndex], _BezierNode);
                if (_bIsBezierNode) {
                    _BezierParser.SetBezierNode(&_BezierNode);
                }
                else {
                    _NurbsParser.SetNurbsNode(&_vecNurbsNodes2D[_nNurbsIndex]);
                }
            }

            double _nKnot = _ScatterNode.nKnot;
            __CUT(_nKnot, _vecNurbsNodes2D[_nNurbsIndex].vecKnots.front(), _vecNurbsNodes2D[_nNurbsIndex].vecKnots.back());

            DPOINT2 _ptPoint, _ptDeriv1, _ptDeriv2;
            if (_bIsBezierNode) {
                _BezierParser.GetBezierPoint(_nKnot, _ptPoint);
            }
            else {
                _NurbsParser.GetNurbsPoint(_nKnot, _ptPoint);
            }

            CheckDPoint2Equal(_ptPoint, _ScatterNode.ptPoint, _nCheckError);

            if (_bIsBezierNode) {
                _BezierParser.GetBezierDeriv(_nKnot, _ptPoint, _ptDeriv1);
            }
            else {
                _NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint, _ptDeriv1);
            }

            CheckDPoint2Equal(_ptPoint, _ScatterNode.ptPoint, _nCheckError);
            CheckDPoint2Equal(_ptDeriv1, _ScatterNode.ptDeriv1, _nCheckError);

            if (_vecNurbsNodes2D[_nNurbsIndex].nDegree > 1) {
                if (_bIsBezierNode) {
                    _BezierParser.GetBezierDeriv(_nKnot, _ptPoint, _ptDeriv1, _ptDeriv2);
                }
                else {
                    _NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint, _ptDeriv1, _ptDeriv2);
                }

                CheckDPoint2Equal(_ptPoint, _ScatterNode.ptPoint, _nCheckError);
                CheckDPoint2Equal(_ptDeriv1, _ScatterNode.ptDeriv1, _nCheckError);
                CheckDPoint2Equal(_ptDeriv2, _ScatterNode.ptDeriv2, _nCheckError);
            }
        }
    }
    else {
        CNurbsParser3D _NurbsParser;
        CBezierParser3D _BezierParser;
        BezierNode3D _BezierNode;
        size_t _nNurbsIndex = 0;
        for (const auto& _ScatterNode : _vecParserResult) {
            size_t _nIndex = _nNurbsIndex;
            ASSERT_TRUE(_nIndex < _vecNurbsNodes.size());
            ASSERT_TRUE(_ScatterNode.nKnot > _vecNurbsNodes[_nIndex].vecKnots.front() - Precision::RealTolerance());
            while (_ScatterNode.nKnot > _vecNurbsNodes[_nIndex].vecKnots.back() + Precision::RealTolerance()) {
                ++_nIndex;
                ASSERT_TRUE(_nIndex < _vecNurbsNodes.size());
            }

            bool _bIsBezierNode = false;
            if (_nIndex == 0 || _nIndex != _nNurbsIndex) {
                _nNurbsIndex = _nIndex;
                _bIsBezierNode = TransformNurbsNode(_vecNurbsNodes[_nNurbsIndex], _BezierNode);
                if (_bIsBezierNode) {
                    _BezierParser.SetBezierNode(&_BezierNode);
                }
                else {
                    _NurbsParser.SetNurbsNode(&_vecNurbsNodes[_nNurbsIndex]);
                }
            }

            double _nKnot = _ScatterNode.nKnot;
            __CUT(_nKnot, _vecNurbsNodes[_nNurbsIndex].vecKnots.front(), _vecNurbsNodes[_nNurbsIndex].vecKnots.back());

            DPOINT3 _ptPoint, _ptDeriv1, _ptDeriv2;
            if (_bIsBezierNode) {
                _BezierParser.GetBezierPoint(_nKnot, _ptPoint);
            }
            else {
                _NurbsParser.GetNurbsPoint(_nKnot, _ptPoint);
            }

            CheckDPoint3Equal(_ptPoint, _ScatterNode.ptPoint, _nCheckError);

            if (_bIsBezierNode) {
                _BezierParser.GetBezierDeriv(_nKnot, _ptPoint, _ptDeriv1);
            }
            else {
                _NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint, _ptDeriv1);
            }

            CheckDPoint3Equal(_ptPoint, _ScatterNode.ptPoint, _nCheckError);
            CheckDPoint3Equal(_ptDeriv1, _ScatterNode.ptDeriv1, _nCheckError);

            if (_vecNurbsNodes[_nNurbsIndex].nDegree > 1) {
                if (_bIsBezierNode) {
                    _BezierParser.GetBezierDeriv(_nKnot, _ptPoint, _ptDeriv1, _ptDeriv2);
                }
                else {
                    _NurbsParser.GetNurbsDeriv(_nKnot, _ptPoint, _ptDeriv1, _ptDeriv2);
                }

                CheckDPoint3Equal(_ptPoint, _ScatterNode.ptPoint, _nCheckError);
                CheckDPoint3Equal(_ptDeriv1, _ScatterNode.ptDeriv1, _nCheckError);
                CheckDPoint3Equal(_ptDeriv2, _ScatterNode.ptDeriv2, _nCheckError);
            }
        }
    }
}

INSTANTIATE_TEST_CASE_P(NeatParserPT, CNurbsParserTest, testing::ValuesIn(gs_listTestFileInfo));

//////////////////////////////////////////////////////////////////////////
// local function
static void ReadParserResult(const std::string& strFilePath_, std::vector<ScatterNode3D>& vecParserResult_)
{
    if (strFilePath_.empty()) {
        printf("Invalid file path!\n");
        return;
    }

    std::ifstream _infile;
    _infile.open(strFilePath_.c_str(), std::fstream::in);
    if (!_infile) {
        printf("Open file (%s) failed!\n", strFilePath_.c_str());
        return;
    }

    vecParserResult_.clear();

    std::string _strBuffer;
    while (getline(_infile, _strBuffer)) {
        ScatterNode3D _ScatterNode;
        std::istringstream _instring(_strBuffer);
        if (_instring >> _ScatterNode.nKnot
            >> _ScatterNode.ptPoint[axis::x] >> _ScatterNode.ptPoint[axis::y] >> _ScatterNode.ptPoint[axis::z]
            >> _ScatterNode.ptDeriv1[axis::x] >> _ScatterNode.ptDeriv1[axis::y] >> _ScatterNode.ptDeriv1[axis::z]
            >> _ScatterNode.ptDeriv2[axis::x] >> _ScatterNode.ptDeriv2[axis::y] >> _ScatterNode.ptDeriv2[axis::z]) {
            vecParserResult_.emplace_back(_ScatterNode);
        }
    }
}