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

/**
 * This file contains the 'main' function. Program execution begins and ends there.
 */
int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

//////////////////////////////////////////////////////////////////////////
// global function
void CheckDPoint2Equal(const neat::DPOINT2& ptLeft_, const neat::DPOINT2& ptRight_, double nTolerance_)
{
    EXPECT_NEAR(ptLeft_[axis::x], ptRight_[axis::x], nTolerance_);
    EXPECT_NEAR(ptLeft_[axis::y], ptRight_[axis::y], nTolerance_);
}

void CheckDPoint3Equal(const neat::DPOINT3& ptLeft_, const neat::DPOINT3& ptRight_, double nTolerance_)
{
    EXPECT_NEAR(ptLeft_[axis::x], ptRight_[axis::x], nTolerance_);
    EXPECT_NEAR(ptLeft_[axis::y], ptRight_[axis::y], nTolerance_);
    EXPECT_NEAR(ptLeft_[axis::z], ptRight_[axis::z], nTolerance_);
}

bool ReadNurbsFromFile(const std::string& strFilePath_, std::vector<neat::NurbsNode3D>& vecNurbsNodes_)
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

    std::string _strBuf;
    while (getline(_infile, _strBuf)) {
        std::istringstream _instring(_strBuf);
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

bool WritePointsToFile(const std::string& strFilePath_, const neat::VECDPOINT3& vecPoints_)
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

//////////////////////////////////////////////////////////////////////////
// class CNurbsFileInfoList
CNurbsFileInfoList::CNurbsFileInfoList()
{
    const static std::string _s_strFileName[] = { "butterfly.nc", "egg.nc", "mouse.nc", "rectangle.nc", "tube.nc", "wave.nc" };
    const static std::string _s_strFileDir = (_access(R"(.\TestData\)", 0) != -1)
        ? R"(.\TestData\)"
        : (_access(R"(..\TestData\)", 0) != -1 ? R"(..\TestData\)" : R"(..\..\..\TestData\)");

    for (int i = 0; i < _countof(_s_strFileName); ++i) {
        this->emplace_back(NurbsFileInfo{ _s_strFileDir, _s_strFileName[i] });
    }
}