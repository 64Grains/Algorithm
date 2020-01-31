/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __RBSPLINE_PARSER_H_20200130__
#define __RBSPLINE_PARSER_H_20200130__

/**
 * This file defines the parser for rational B-spline curve.
 */
namespace neat
{
    /**
     * This class only calculates less than first-order derivative.
     */
    template<typename TYPE>
    class CRBSplineParser
    {
    public:
        CRBSplineParser();
        ~CRBSplineParser();

        // Set the information for nurbs node.
        void SetNurbsNode(const NurbsNode<TYPE>* pNurbsNode_, bool bRational_);

        // Returns in ptPoint_ the point of parameter nKnot_.
        void GetNurbsPoint(double nKnot_, TYPE& ptPoint_);

        // Returns in ptPoint_ the point and ptDeriv1_ the first-order derivative of parameter nKnot_.
        void GetNurbsDeriv(double nKnot_, TYPE& ptPoint_, TYPE& ptDeriv1_);

    private:
        // Record valid poles and their weights.
        void RecordValidPoles(int nKnotIndex_);

        // De-Boor recursive algorithm to calculate iteration poles and weights.
        void DeBoorAlgorithm(double nKnot_, int nKnotIndex_, int nStartIndex_, int nEndIndex_);

    private:
        NurbsNode<TYPE>* m_pNurbsNode;
        int m_nStartIndex;
        int m_nEndIndex;
        int m_nKnotIndex;
        bool m_bRational;

        // Record the effective control points corresponding to m_nKnotIndex
        CFastVector<TYPE> m_vecValidPoles;
        CFastVector<TYPE> m_vecTempPoles1;
        CFastVector<TYPE> m_vecTempPoles2;

        // Record the effective control point weight corresponding to m_nKnotIndex
        CFastVector<double> m_vecValidWeights;
        CFastVector<double> m_vecTempWeights1;
        CFastVector<double> m_vecTempWeights2;
    };

    /**
     * The implementation of template class.
     */
    template<typename TYPE>
    CRBSplineParser<TYPE>::CRBSplineParser()
        : m_pNurbsNode(nullptr)
        , m_nStartIndex(0)
        , m_nEndIndex(0)
        , m_nKnotIndex(0)
        , m_bRational(false)
    {
    }

    template<typename TYPE>
    CRBSplineParser<TYPE>::~CRBSplineParser()
    {
    }

    template<typename TYPE>
    void CRBSplineParser<TYPE>::SetNurbsNode(const NurbsNode<TYPE>* pNurbsNode_, bool bRational_)
    {
        NEAT_RAISE_IF(!bRational_ || !CheckNurbsNodeRational(pNurbsNode_), g_strNurbsParamsError);

        m_pNurbsNode = (NurbsNode<TYPE>*)pNurbsNode_;
        m_bRational = bRational_;

        // Initialize the knot index limit value
        m_nStartIndex = m_pNurbsNode->nDegree;
        m_nEndIndex = static_cast<int>(m_pNurbsNode->vecKnots.size()) - m_pNurbsNode->nDegree - 1;
        NEAT_RAISE(DOUBLE_EQU(m_pNurbsNode->vecKnots[m_nStartIndex], m_pNurbsNode->vecKnots[m_nEndIndex]), g_strNurbsParamsError);

        m_nKnotIndex = m_nStartIndex;
        while (DOUBLE_EQU(m_pNurbsNode->vecKnots[m_nKnotIndex + 1], m_pNurbsNode->vecKnots[m_nStartIndex])) {
            ++m_nKnotIndex;
        }

        RecordValidPoles(m_nKnotIndex);
    }

    template<typename TYPE>
    void CRBSplineParser<TYPE>::GetNurbsPoint(double nKnot_, TYPE& ptPoint_)
    {
        NEAT_RAISE_IF(m_pNurbsNode == nullptr, g_strEmptyPointer);
        NEAT_RAISE_IF(!CheckNurbsNodeRational(m_pNurbsNode), g_strNurbsParamsError);

        int _nLastKnotIndex = m_nKnotIndex;
        int _nCurKnotIndex = FindSpanForKnot(m_pNurbsNode->vecKnots, nKnot_, m_nStartIndex, m_nEndIndex, m_nKnotIndex);
        if (_nCurKnotIndex != _nLastKnotIndex) {
            RecordValidPoles(_nCurKnotIndex);
        }

        m_vecTempPoles1.clear();
        m_vecTempPoles2.clear();
        m_vecTempWeights1.clear();
        m_vecTempWeights2.clear();
        m_vecTempPoles1.copy(m_vecValidPoles);
        m_vecTempPoles2.copy(m_vecValidPoles);
        m_vecTempWeights1.copy(m_vecValidWeights);
        m_vecTempWeights2.copy(m_vecValidWeights);

        DeBoorAlgorithm(nKnot_, _nCurKnotIndex, 0, m_pNurbsNode->nDegree);
        ptPoint_ = m_vecTempPoles1[m_pNurbsNode->nDegree];
    }

    template<typename TYPE>
    void CRBSplineParser<TYPE>::GetNurbsDeriv(double nKnot_, TYPE& ptPoint_, TYPE& ptDeriv1_)
    {
        NEAT_RAISE_IF(m_pNurbsNode == nullptr, g_strEmptyPointer);
        NEAT_RAISE_IF(!CheckNurbsNodeRational(m_pNurbsNode), g_strNurbsParamsError);

        int _nLastKnotIndex = m_nKnotIndex;
        int _nCurKnotIndex = FindSpanForKnot(m_pNurbsNode->vecKnots, nKnot_, m_nStartIndex, m_nEndIndex, m_nKnotIndex);
        if (_nCurKnotIndex != _nLastKnotIndex) {
            RecordValidPoles(_nCurKnotIndex);
        }

        m_vecTempPoles1.clear();
        m_vecTempPoles2.clear();
        m_vecTempWeights1.clear();
        m_vecTempWeights2.clear();
        m_vecTempPoles1.copy(m_vecValidPoles);
        m_vecTempPoles2.copy(m_vecValidPoles);
        m_vecTempWeights1.copy(m_vecValidWeights);
        m_vecTempWeights2.copy(m_vecValidWeights);

        const int& _nDegree = m_pNurbsNode->nDegree;
        const VECDOUBLE& _vecKnots = m_pNurbsNode->vecKnots;

        DeBoorAlgorithm(nKnot_, _nCurKnotIndex, 0, _nDegree - 1);

        double _nAlpha = 1.0 / (_vecKnots[_nCurKnotIndex + 1] - _vecKnots[_nCurKnotIndex]);
        double _nBeta = (nKnot_ - _vecKnots[_nCurKnotIndex]) * _nAlpha;
        double _nWeight = (1.0 - _nBeta) * m_vecTempWeights1[_nDegree - 1] + _nBeta * m_vecTempWeights1[_nDegree];
        ptDeriv1_ = (m_vecTempPoles1[_nDegree] - m_vecTempPoles1[_nDegree - 1]) * static_cast<double>(_nDegree) * _nAlpha
            * m_vecTempWeights1[_nDegree] * m_vecTempWeights1[_nDegree - 1] / (_nWeight * _nWeight);

        DeBoorAlgorithm(nKnot_, _nCurKnotIndex, _nDegree - 1, _nDegree);
        ptPoint_ = m_vecTempPoles1[_nDegree];
    }

    template<typename TYPE>
    void CRBSplineParser<TYPE>::RecordValidPoles(int nKnotIndex_)
    {
        NEAT_RAISE_IF(nKnotIndex_ < m_nStartIndex || nKnotIndex_ > m_nEndIndex, g_strNurbsParamsError);

        m_vecValidPoles.clear();
        auto _iterPolesBegin = m_pNurbsNode->vecPoles.begin() + nKnotIndex_ - m_pNurbsNode->nDegree;
        auto _iterPolesEnd = m_pNurbsNode->vecPoles.begin() + nKnotIndex_ + 1;
        for (; _iterPolesBegin != _iterPolesEnd; ++_iterPolesBegin) {
            m_vecValidPoles.push_back(*_iterPolesBegin);
        }

        m_vecValidWeights.clear();
        auto _iterWeightBegin = m_pNurbsNode->vecWeights.begin() + nKnotIndex_ - m_pNurbsNode->nDegree;
        auto _iterWeightEnd = m_pNurbsNode->vecWeights.begin() + nKnotIndex_ + 1;
        for (; _iterWeightBegin != _iterWeightEnd; ++_iterWeightBegin) {
            m_vecValidWeights.push_back(*_iterWeightBegin);
        }
    }

    template<typename TYPE>
    void CRBSplineParser<TYPE>::DeBoorAlgorithm(double nKnot_, int nKnotIndex_, int nStartIndex_, int nEndIndex_)
    {
        NEAT_RAISE_IF(nStartIndex_ >= nEndIndex_, g_strNurbsParamsError);

        const int& _nDegree = m_pNurbsNode->nDegree;
        const VECDOUBLE& _vecKnots = m_pNurbsNode->vecKnots;

        for (int i = nStartIndex_; i < nEndIndex_; ++i) {
            for (int j = i; j < _nDegree; ++j) {
                int _nTempIndex = nKnotIndex_ - _nDegree + j + 1;
                double _nAlpha = (nKnot_ - _vecKnots[_nTempIndex]) / (_vecKnots[_nDegree + _nTempIndex - i] - _vecKnots[_nTempIndex]);
                m_vecTempWeights2[j + 1] = (1.0 - _nAlpha) * m_vecTempWeights1[j] + _nAlpha * m_vecTempWeights1[j + 1];
                m_vecTempPoles2[j + 1] = m_vecTempPoles1[j] * (1.0 - _nAlpha) * m_vecTempWeights1[j] / m_vecTempWeights2[j + 1]
                    + m_vecTempPoles1[j + 1] * _nAlpha * m_vecTempWeights1[j + 1] / m_vecTempWeights2[j + 1];
            }

            for (int k = i + 1; k <= _nDegree; ++k) {
                m_vecTempPoles1[k] = m_vecTempPoles2[k];
                m_vecTempWeights1[k] = m_vecTempWeights2[k];
            }
        }
    }

} // End namespace neat

#endif // __RBSPLINE_PARSER_H_20200130__