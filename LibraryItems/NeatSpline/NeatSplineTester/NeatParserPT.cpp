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
 * This file parameter tests the interface for parsing splines.
 */
const static CNurbsFileInfoList gs_listNurbsFileInfo;

class CNurbsParserTest : public ::testing::TestWithParam<NurbsFileInfo>
{
};

TEST_P(CNurbsParserTest, NurbsFiles)
{
    const NurbsFileInfo& _fileInfo = GetParam();

    std::vector<NurbsNode3D> _vecNurbsNodes;
    ReadNurbsFromFile(_fileInfo.strFileDir + _fileInfo.strFileName, _vecNurbsNodes);

    CNurbsParser3D _NurbsParser;
    VECDPOINT3 _vecDeriv0s[3];
    VECDPOINT3 _vecDeriv1s[2];
    VECDPOINT3 _vecDeriv2s;

    std::string _strFilePrefix = _fileInfo.strFileDir;
    _strFilePrefix += _fileInfo.strFileName.substr(0, _fileInfo.strFileName.find_last_of('.'));
    _strFilePrefix += "-NurbsParser-";

    std::string _strFileSuffix(".txt");
    std::string _strFileName;

    size_t _nMaxCount = 200;
    for (size_t i = 0, _nSize = _vecNurbsNodes.size(); i < _nSize; ++i) {
        _NurbsParser.SetNurbsNode(&_vecNurbsNodes[i]);

        for (size_t j = 0; j < 3; ++j) {
            _vecDeriv0s[j].clear();
            _vecDeriv0s[j].resize(_nMaxCount + 1);
        }

        for (size_t j = 0; j < 2; ++j) {
            _vecDeriv1s[j].clear();
            _vecDeriv1s[j].resize(_nMaxCount + 1);
        }

        _vecDeriv2s.clear();
        _vecDeriv2s.resize(_nMaxCount + 1);

        const NurbsNode3D& _NurbsNode = _vecNurbsNodes[i];
        double _nStartKnot = _NurbsNode.vecKnots[_NurbsNode.nDegree];
        double _nEndKnot = _NurbsNode.vecKnots[_NurbsNode.vecKnots.size() - _NurbsNode.nDegree - 1];
        for (size_t j = 0; j <= _nMaxCount; ++j) {
            double _nKnot = _nStartKnot + (_nEndKnot - _nStartKnot) / _nMaxCount * j;
            _NurbsParser.GetNurbsPoint(_nKnot, _vecDeriv0s[0][j]);
            _NurbsParser.GetNurbsDeriv(_nKnot, _vecDeriv0s[1][j], _vecDeriv1s[0][j]);
            _NurbsParser.GetNurbsDeriv(_nKnot, _vecDeriv0s[2][j], _vecDeriv1s[1][j], _vecDeriv2s[j]);
        }

        // output
        for (int j = 0; j < 3; ++j) {
            _strFileName = _strFilePrefix + "Func" + std::string(1, '0' + j) + "-Deriv0" + _strFileSuffix;
            WritePointsToFile(_strFileName, _vecDeriv0s[j]);
        }

        for (int j = 0; j < 2; ++j) {
            _strFileName = _strFilePrefix + "Func" + std::string(1, '0' + j + 1) + "-Deriv1" + _strFileSuffix;
            WritePointsToFile(_strFileName, _vecDeriv1s[j]);
        }

        _strFileName = _strFilePrefix + "Func2-Deriv2" + _strFileSuffix;
        WritePointsToFile(_strFileName, _vecDeriv2s);
    }
}

INSTANTIATE_TEST_CASE_P(NeatParserPT, CNurbsParserTest, testing::ValuesIn(gs_listNurbsFileInfo));