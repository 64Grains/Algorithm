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
 * This file parameter tests the interface for converting splines.
 */
const static CNurbsFileInfoList gs_listNurbsFileInfo;

class CNurbsConverterTest : public ::testing::TestWithParam<NurbsFileInfo>
{
};

TEST_P(CNurbsConverterTest, NurbsFiles)
{
    const NurbsFileInfo& _fileInfo = GetParam();

    std::vector<NurbsNode3D> _vecNurbsNodes;
    ReadNurbsFromFile(_fileInfo.strFileDir + _fileInfo.strFileName, _vecNurbsNodes);

    std::string _strFileName = _fileInfo.strFileDir;
    _strFileName += _fileInfo.strFileName.substr(0, _fileInfo.strFileName.find_last_of('.'));
    _strFileName += "-NurbsTranslator.txt";

    FILE* _pFile = nullptr;
    ASSERT_TRUE(fopen_s(&_pFile, _strFileName.c_str(), "w") == 0);
    ASSERT_TRUE(_pFile != nullptr);

    std::vector<BezierNode3D> _vecBezierNodes;
    for (size_t i = 0, _nSize = _vecNurbsNodes.size(); i < _nSize; ++i) {
        ConvertNurbsToBezier(&_vecNurbsNodes[i], _vecBezierNodes);

        for (size_t j = 0, _nCount = _vecBezierNodes.size(); j < _nCount; ++j) {
            BezierNode3D& _nodeBezier = _vecBezierNodes[j];

            // check
            ASSERT_TRUE(_nodeBezier.vecPoles.size() == _vecNurbsNodes[i].nDegree + 1);
            ASSERT_TRUE(_nodeBezier.vecWeights.empty() || _nodeBezier.vecWeights.size() == _vecNurbsNodes[i].nDegree + 1);

            // output
            if (_nodeBezier.vecWeights.empty()) {
                fprintf(_pFile, "G6.2 P%zd K%f X%f Y%f Z%f\n",
                    _nodeBezier.vecPoles.size() - 1,
                    _nodeBezier.nStartKnot,
                    _nodeBezier.vecPoles.front()[0],
                    _nodeBezier.vecPoles.front()[1],
                    _nodeBezier.vecPoles.front()[2]);

                for (size_t k = 1, _nPoleSize = _nodeBezier.vecPoles.size(); k < _nPoleSize; ++k) {
                    fprintf(_pFile, "K%f X%f Y%f Z%f\n",
                        _nodeBezier.nStartKnot,
                        _nodeBezier.vecPoles[k][0],
                        _nodeBezier.vecPoles[k][1],
                        _nodeBezier.vecPoles[k][2]);
                }

                for (size_t k = 0, _nPoleSize = _nodeBezier.vecPoles.size(); k < _nPoleSize; ++k) {
                    fprintf(_pFile, "K%f\n", _nodeBezier.nEndKnot);
                }
            }
            else {
                fprintf(_pFile, "G6.2 P%zd K%f X%f Y%f Z%f R%f\n",
                    _nodeBezier.vecPoles.size() - 1,
                    _nodeBezier.nStartKnot,
                    _nodeBezier.vecPoles.front()[0],
                    _nodeBezier.vecPoles.front()[1],
                    _nodeBezier.vecPoles.front()[2],
                    _nodeBezier.vecWeights.front());

                for (size_t k = 1, _nPoleSize = _nodeBezier.vecPoles.size(); k < _nPoleSize; ++k) {
                    fprintf(_pFile, "K%f X%f Y%f Z%f R%f\n",
                        _nodeBezier.nStartKnot,
                        _nodeBezier.vecPoles[k][0],
                        _nodeBezier.vecPoles[k][1],
                        _nodeBezier.vecPoles[k][2],
                        _nodeBezier.vecWeights[k]);
                }

                for (size_t k = 0, _nPoleSize = _nodeBezier.vecPoles.size(); k < _nPoleSize; ++k) {
                    fprintf(_pFile, "K%f\n", _nodeBezier.nEndKnot);
                }
            }
        }
    }

    fclose(_pFile);
}

INSTANTIATE_TEST_CASE_P(NeatConverterPT, CNurbsConverterTest, testing::ValuesIn(gs_listNurbsFileInfo));