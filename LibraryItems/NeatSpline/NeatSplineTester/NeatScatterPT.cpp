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

/**
 * This file parameter tests the interface for scatting splines.
 */
const static CNurbsFileInfoList gs_listNurbsFileInfo;

class CNurbsScatterTest : public ::testing::TestWithParam<NurbsFileInfo>
{
};

TEST_P(CNurbsScatterTest, NurbsFiles)
{
    const NurbsFileInfo& _fileInfo = GetParam();

    std::vector<NurbsNode3D> _vecNurbsNodes;
    ReadNurbsFromFile(_fileInfo.strFileDir + _fileInfo.strFileName, _vecNurbsNodes);

    double _nDeflection = 0.01;
    VECDPOINT3 _vecScatterPoints;
    std::vector<ScatterNode3D> _vecScatterNodes;

    std::string _strFilePrefix = _fileInfo.strFileDir;
    _strFilePrefix += _fileInfo.strFileName.substr(0, _fileInfo.strFileName.find_last_of('.'));
    _strFilePrefix += "-NurbsScatter-";

    std::string _strFileSuffix(".txt");
    std::string _strFileName;

    for (size_t i = 0, _nSize = _vecNurbsNodes.size(); i < _nSize; ++i) {
        ScatterNurbs(&_vecNurbsNodes[i], _nDeflection, _vecScatterPoints);
        ScatterNurbs(&_vecNurbsNodes[i], _nDeflection, _vecScatterNodes);

        // output
        _strFileName = _strFilePrefix + "Func0-Deriv0" + _strFileSuffix;
        WritePointsToFile(_strFileName, _vecScatterPoints);

        VECDPOINT3 _vecDerivPoints[3];
        for (size_t j = 0; j < 3; ++j) {
            _vecDerivPoints[j].reserve(_vecScatterNodes.size());
        }

        for (size_t j = 0, _nCount = _vecScatterNodes.size(); j < _nCount; ++j) {
            _vecDerivPoints[0].push_back(_vecScatterNodes[j].ptPoint);
            _vecDerivPoints[1].push_back(_vecScatterNodes[j].ptDeriv1);
            _vecDerivPoints[2].push_back(_vecScatterNodes[j].ptDeriv2);
        }

        for (int j = 0; j < 3; ++j) {
            _strFileName = _strFilePrefix + "Func1-Deriv" + std::string(1, '0' + j) + _strFileSuffix;
            WritePointsToFile(_strFileName, _vecDerivPoints[j]);
        }
    }
}

INSTANTIATE_TEST_CASE_P(NeatScatterPT, CNurbsScatterTest, testing::ValuesIn(gs_listNurbsFileInfo));