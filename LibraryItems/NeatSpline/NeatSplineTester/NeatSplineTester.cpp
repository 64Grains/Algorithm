/***************************************************************************************************
 Created on: 2020-01-30
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
#include <io.h>

using namespace neat;
using namespace tester;

/**
 * This file contains the 'main' function. Program execution begins and ends there.
 */
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

//////////////////////////////////////////////////////////////////////////
// class CTestFileInfoList
CTestFileInfoList::CTestFileInfoList()
{
    std::string _strFileDir;
    if (_access(R"(.\TestData\)", 0) != -1) {
        _strFileDir = R"(.\TestData\)";
    }
    else if (_access(R"(..\TestData\)", 0) != -1) {
        _strFileDir = R"(..\TestData\)";
    }
    else {
        _strFileDir = R"(..\..\..\TestData\)";
    }

    // Read test cases from the specified file
    std::string _strRecordFile = _strFileDir + "TestFileList.txt";

    std::ifstream _infile;
    _infile.open(_strRecordFile.c_str(), std::fstream::in);
    if (!_infile) {
        printf("Open file (%s) failed!\n", _strRecordFile.c_str());
        return;
    }

    std::string _strBuffer;
    while (getline(_infile, _strBuffer)) {
        if (_strBuffer.empty()) {
            continue;
        }

        if (_strBuffer.at(0) == '%') {
            // This line is a comment
            continue;
        }

        size_t _nStartPos = _strBuffer.find_first_of('"');
        if (_nStartPos == std::string::npos) {
            continue;
        }

        // Parse test case
        TestFileInfo _TestCase;
        std::string* _szName[5] = {&_TestCase.strFileDir, &_TestCase.strFileName,
            &_TestCase.strParserFile, &_TestCase.strScatterFile, &_TestCase.strConverterFile};

        size_t _nIndex = 0;
        while (true) {
            size_t _nEndPos = _nStartPos + 1;
            while (_nEndPos < _strBuffer.size() && _strBuffer[_nEndPos] != '"') {
                ++_nEndPos;
            }

            if (_nEndPos >= _strBuffer.size()) {
                break;
            }

            NEAT_RAISE(_nIndex >= 5, "Wrong test case!");
            *(_szName[_nIndex++]) = _strBuffer.substr(_nStartPos + 1, _nEndPos - _nStartPos - 1);

            _nStartPos = _nEndPos + 1;
            while (_nStartPos < _strBuffer.size() && _strBuffer[_nStartPos] != '"') {
                ++_nStartPos;
            }
        }

        for (size_t i = 0; i < 5; ++i) {
            NEAT_RAISE(_szName[i]->empty(), "Wrong test case!");
        }

        // Fix directory name
        _TestCase.strFileDir = _strFileDir + _TestCase.strFileDir + "\\";
        this->emplace_back(_TestCase);
    }
}

//////////////////////////////////////////////////////////////////////////
// global function
void tester::CheckDPoint2Equal(const neat::DPOINT2& ptLeft_, const neat::DPOINT2& ptRight_, double nTolerance_)
{
    EXPECT_NEAR(ptLeft_[axis::x], ptRight_[axis::x], nTolerance_);
    EXPECT_NEAR(ptLeft_[axis::y], ptRight_[axis::y], nTolerance_);
}

void tester::CheckDPoint3Equal(const neat::DPOINT3& ptLeft_, const neat::DPOINT3& ptRight_, double nTolerance_)
{
    EXPECT_NEAR(ptLeft_[axis::x], ptRight_[axis::x], nTolerance_);
    EXPECT_NEAR(ptLeft_[axis::y], ptRight_[axis::y], nTolerance_);
    EXPECT_NEAR(ptLeft_[axis::z], ptRight_[axis::z], nTolerance_);
}

bool tester::DowngradeDPoint(const neat::VECDPOINT3& vecPoint3D_, neat::VECDPOINT2& vecPoint2D_)
{
    vecPoint2D_.clear();
    vecPoint2D_.reserve(vecPoint3D_.size());

    bool _bOnPlane = true;
    const double _nZCoor = vecPoint3D_.empty() ? 0.0 : vecPoint3D_.front()[axis::z];
    for (const auto& _ptPoint : vecPoint3D_) {
        if (fabs(_ptPoint[axis::z] - _nZCoor) > Precision::RealTolerance()) {
            _bOnPlane = false;
            break;
        }

        vecPoint2D_.emplace_back(DPOINT2(_ptPoint[axis::x], _ptPoint[axis::y]));
    }

    return _bOnPlane;
}

bool tester::DowngradeNurbsNode(const std::vector<neat::NurbsNode3D>& vecNurbs3D_, std::vector<neat::NurbsNode2D>& vecNurbs2D_)
{
    vecNurbs2D_.clear();
    vecNurbs2D_.reserve(vecNurbs3D_.size());

    bool _bOnPlane = true;
    const double _nZCoor = vecNurbs3D_.empty() ? 0.0
        : (vecNurbs3D_.front().vecPoles.empty() ? 0.0 : vecNurbs3D_.front().vecPoles.front()[axis::z]);
    for (const auto& _Nurbs3D : vecNurbs3D_) {
        NurbsNode2D _Nurbs2D;
        for (const auto& _ptPoint : _Nurbs3D.vecPoles) {
            if (fabs(_ptPoint[axis::z] - _nZCoor) > Precision::RealTolerance()) {
                _bOnPlane = false;
                break;
            }

            _Nurbs2D.vecPoles.emplace_back(DPOINT2(_ptPoint[axis::x], _ptPoint[axis::y]));
        }

        if (!_bOnPlane) {
            break;
        }

        _Nurbs2D.nDegree = _Nurbs3D.nDegree;
        _Nurbs2D.vecKnots = _Nurbs3D.vecKnots;
        _Nurbs2D.vecWeights = _Nurbs3D.vecWeights;
        vecNurbs2D_.emplace_back(_Nurbs2D);
    }

    return _bOnPlane;
}

bool tester::DowngradeBezierNode(const std::vector<neat::BezierNode3D>& vecBezier3D_, std::vector<neat::BezierNode2D>& vecBezier2D_)
{
    vecBezier2D_.clear();
    vecBezier2D_.reserve(vecBezier3D_.size());

    bool _bOnPlane = true;
    const double _nZCoor = vecBezier3D_.empty() ? 0.0
        : (vecBezier3D_.front().vecPoles.empty() ? 0.0 : vecBezier3D_.front().vecPoles.front()[axis::z]);
    for (const auto& _Bezier3D : vecBezier3D_) {
        BezierNode2D _Bezier2D;
        for (const auto& _ptPoint : _Bezier3D.vecPoles) {
            if (fabs(_ptPoint[axis::z] - _nZCoor) > Precision::RealTolerance()) {
                _bOnPlane = false;
                break;
            }

            _Bezier2D.vecPoles.emplace_back(DPOINT2(_ptPoint[axis::x], _ptPoint[axis::y]));
        }

        if (!_bOnPlane) {
            break;
        }

        _Bezier2D.nStartKnot = _Bezier3D.nStartKnot;
        _Bezier2D.nEndKnot = _Bezier3D.nEndKnot;
        _Bezier2D.vecWeights = _Bezier3D.vecWeights;
        vecBezier2D_.emplace_back(_Bezier2D);
    }

    return _bOnPlane;
}

bool tester::DowngradeScatterNode(const std::vector<neat::ScatterNode3D>& vecScatter3D_, std::vector<neat::ScatterNode2D>& vecScatter2D_)
{
    vecScatter2D_.clear();
    vecScatter2D_.reserve(vecScatter3D_.size());

    bool _bOnPlane = true;
    const double _nZCoor = vecScatter3D_.empty() ? 0.0 : vecScatter3D_.front().ptPoint[axis::z];
    for (const auto& _Scatter3D : vecScatter3D_) {
        if (fabs(_Scatter3D.ptPoint[axis::z] - _nZCoor) > Precision::RealTolerance()) {
            _bOnPlane = false;
            break;
        }
        else if (fabs(_Scatter3D.ptDeriv1[axis::z]) > Precision::RealTolerance()) {
            _bOnPlane = false;
            break;
        }
        else if (fabs(_Scatter3D.ptDeriv2[axis::z]) > Precision::RealTolerance()) {
            _bOnPlane = false;
            break;
        }

        ScatterNode2D _Scatter2D;
        _Scatter2D.nKnot = _Scatter3D.nKnot;
        _Scatter2D.ptPoint = DPOINT2(_Scatter3D.ptPoint[axis::x], _Scatter3D.ptPoint[axis::y]);
        _Scatter2D.ptDeriv1 = DPOINT2(_Scatter3D.ptDeriv1[axis::x], _Scatter3D.ptDeriv1[axis::y]);
        _Scatter2D.ptDeriv2 = DPOINT2(_Scatter3D.ptDeriv2[axis::x], _Scatter3D.ptDeriv2[axis::y]);
        vecScatter2D_.emplace_back(_Scatter2D);
    }

    return _bOnPlane;
}

bool tester::TransformNurbsNode(const neat::NurbsNode2D& NurbsNode_, neat::BezierNode2D& BezierNode_)
{
    if (NurbsNode_.vecKnots.size() != 2 * (NurbsNode_.nDegree + 1)) {
        return false;
    }

    BezierNode_.nStartKnot = NurbsNode_.vecKnots[NurbsNode_.nDegree];
    BezierNode_.nEndKnot = NurbsNode_.vecKnots[NurbsNode_.nDegree + 1];
    BezierNode_.vecPoles = NurbsNode_.vecPoles;
    BezierNode_.vecWeights = NurbsNode_.vecWeights;

    return true;
}

bool tester::TransformNurbsNode(const neat::NurbsNode3D& NurbsNode_, neat::BezierNode3D& BezierNode_)
{
    if (NurbsNode_.vecKnots.size() != 2 * (NurbsNode_.nDegree + 1)) {
        return false;
    }

    BezierNode_.nStartKnot = NurbsNode_.vecKnots[NurbsNode_.nDegree];
    BezierNode_.nEndKnot = NurbsNode_.vecKnots[NurbsNode_.nDegree + 1];
    BezierNode_.vecPoles = NurbsNode_.vecPoles;
    BezierNode_.vecWeights = NurbsNode_.vecWeights;

    return true;
}

bool tester::ReadNurbsFromFile(const std::string& strFilePath_, std::vector<neat::NurbsNode3D>& vecNurbsNodes_)
{
    if (strFilePath_.empty()) {
        printf("Invalid file path!\n");
        return false;
    }

    std::ifstream _infile;
    _infile.open(strFilePath_.c_str(), std::fstream::in);
    if (!_infile) {
        printf("Open file (%s) failed!\n", strFilePath_.c_str());
        return false;
    }

    NurbsNode3D _nurbsNode;
    DPOINT3 _ptPoint;
    bool _bGFlag = false;
    bool _bAxis = false;

    std::string _strBuffer;
    while (getline(_infile, _strBuffer)) {
        std::istringstream _instring(_strBuffer);
        char _nFlag;
        double _nValue;
        while (_instring >> _nFlag >> _nValue) {
            switch (_nFlag) {
            case 'g':
            case 'G':
                if (_bGFlag) {
                    vecNurbsNodes_.push_back(_nurbsNode);
                    _bGFlag = (6.2 == _nValue) ? true : false;
                    _nurbsNode = NurbsNode3D();
                }
                else if (6.2 == _nValue) {
                    _bGFlag = true;
                    _nurbsNode = NurbsNode3D();
                }
                else {
                    _bGFlag = false;
                }
                break;
            case 'p':
            case 'P':
                _nurbsNode.nDegree = static_cast<int>(_nValue);
                break;
            case 'k':
            case 'K':
                if (_bGFlag) {
                    _nurbsNode.vecKnots.push_back(_nValue);
                }
                break;
            case 'x':
            case 'X':
                _ptPoint[0] = _nValue;
                _bAxis = true;
                break;
            case 'y':
            case 'Y':
                _ptPoint[1] = _nValue;
                _bAxis = true;
                break;
            case 'z':
            case 'Z':
                _ptPoint[2] = _nValue;
                _bAxis = true;
                break;
            case 'r':
            case 'R':
                if (_bGFlag) {
                    _nurbsNode.vecWeights.push_back(_nValue);
                }
                break;
            default:
                NEAT_RAISE(true, "Unknown error!");
                break;
            }
        }

        if (_bGFlag && _bAxis) {
            _nurbsNode.vecPoles.push_back(_ptPoint);
        }

        _bAxis = false;
    }

    if (_bGFlag) {
        vecNurbsNodes_.push_back(_nurbsNode);
    }

    return true;
}

bool tester::WritePointsToFile(const std::string& strFilePath_, const neat::VECDPOINT3& vecPoints_)
{
    if (strFilePath_.empty()) {
        printf("Invalid file path!\n");
        return false;
    }

    if (vecPoints_.empty()) {
        printf("No data!\n");
        return false;
    }

    FILE* _pFile = nullptr;
    if (fopen_s(&_pFile, strFilePath_.c_str(), "a") || !_pFile) {
        printf("Open file (%s) failed!\n", strFilePath_.c_str());
        return false;
    }

    fprintf(_pFile, "G00 X%f Y%f Z%f\n", vecPoints_[0][axis::x], vecPoints_[0][axis::y], vecPoints_[0][axis::z]);
    for (size_t i = 1, _nSize = vecPoints_.size(); i < _nSize; ++i) {
        fprintf(_pFile, "G01 X%f Y%f Z%f\n", vecPoints_[i][axis::x], vecPoints_[i][axis::y], vecPoints_[i][axis::z]);
    }

    fclose(_pFile);
    return true;
}

bool tester::WritePointsToFile(const std::string& strFilePath_, const neat::VECDPOINT2& vecPoints_)
{
    if (strFilePath_.empty()) {
        printf("Invalid file path!\n");
        return false;
    }

    if (vecPoints_.empty()) {
        printf("No data!\n");
        return false;
    }

    FILE* _pFile = nullptr;
    if (fopen_s(&_pFile, strFilePath_.c_str(), "a") || !_pFile) {
        printf("Open file (%s) failed!\n", strFilePath_.c_str());
        return false;
    }

    fprintf(_pFile, "G00 X%f Y%f\n", vecPoints_[0][axis::x], vecPoints_[0][axis::y]);
    for (size_t i = 1, _nSize = vecPoints_.size(); i < _nSize; ++i) {
        fprintf(_pFile, "G01 X%f Y%f\n", vecPoints_[i][axis::x], vecPoints_[i][axis::y]);
    }

    fclose(_pFile);
    return true;
}

bool tester::WritePolylineToFile(const std::string& strFilePath_, const neat::Polyline2D& Polyline2D_)
{
    if (strFilePath_.empty()) {
        printf("Invalid file path!\n");
        return false;
    }

    if (Polyline2D_.vecNodes.empty()) {
        printf("No data!\n");
        return false;
    }

    FILE* _pFile = nullptr;
    if (fopen_s(&_pFile, strFilePath_.c_str(), "w") != 0 || !_pFile) {
        printf("Open file (%s) failed!\n", strFilePath_.c_str());
        return false;
    }

    fprintf(_pFile, "G00 X%f Y%f\n", Polyline2D_.nStartX, Polyline2D_.nStartY);
    for (size_t i = 0, _nSize = Polyline2D_.vecNodes.size(); i < _nSize; ++i) {
        const Polyline2D::Node2D& _node = Polyline2D_.vecNodes[i];
        if (fabs(_node.nBulge) < Precision::RealTolerance()) {
            fprintf(_pFile, "G01 X%f Y%f\n", _node.nEndX, _node.nEndY);
        }
        else {
            DPOINT2 _ptStart = (i == 0)
                ? DPOINT2(Polyline2D_.nStartX, Polyline2D_.nStartY)
                : DPOINT2(Polyline2D_.vecNodes[i - 1].nEndX, Polyline2D_.vecNodes[i - 1].nEndY);
            DPOINT2 _ptEnd(Polyline2D_.vecNodes[i].nEndX, Polyline2D_.vecNodes[i].nEndY);
            ArcNode2D _ArcNode;
            RetrieveArcNode2D(_ptStart, _ptEnd, Polyline2D_.vecNodes[i].nBulge, _ArcNode);
            if (_ArcNode.nEndAngle > _ArcNode.nStartAngle) {
                fprintf(_pFile, "G03 X%f Y%f R%f\n", _ptEnd[axis::x], _ptEnd[axis::y], _ArcNode.nRadius);
            }
            else {
                fprintf(_pFile, "G02 X%f Y%f R%f\n", _ptEnd[axis::x], _ptEnd[axis::y], _ArcNode.nRadius);
            }
        }
    }

    fclose(_pFile);
    return true;
}