/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __RBSPLINE_PARSER2_H_20200130__
#define __RBSPLINE_PARSER2_H_20200130__

/**
 * This file defines the parser for rational B-spline curve.
 */
namespace neat
{
    /**
     * This class calculates second-order derivative.
     */
    template<typename TYPE>
    class CRBSplineParser2
    {
    public:
        CRBSplineParser2();
        ~CRBSplineParser2();

        // Set the information for nurbs node.
        void SetNurbsNode(const NurbsNode<TYPE>* pNurbsNode_, bool bRational_);

        // Returns in ptPoint_ the point, ptDeriv1_ the first-order derivative and ptDeriv2_ the second-order derivative of parameter nKnot_.
        void GetNurbsDeriv(double nKnot_, TYPE& ptPoint_, TYPE& ptDeriv1_, TYPE& ptDeriv2_);

    private:
        // Record valid weighted poles and their weights.
        void RecordValidPoles(int nKnotIndex_);

        // De-Boor recursive algorithm to calculate iteration weighted poles and weights.
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

        // The weighted poles
        CFastVector<TYPE> m_vecWeightedPoles;

        // The derivation for numerator used to calculate the second-order derivative of Rational BSpline curve
        TYPE m_nxNumeratorDeriv[3];

        // The derivation for denominator used to calculate the second-order derivative of Rational BSpline curve
        double m_nxDenominatorDeriv[3];
    };

    /**
     * The implementation of template class.
     */
    template<typename TYPE>
    CRBSplineParser2<TYPE>::CRBSplineParser2()
        : m_pNurbsNode(nullptr)
        , m_nStartIndex(0)
        , m_nEndIndex(0)
        , m_nKnotIndex(0)
        , m_bRational(false)
    {
    }

    template<typename TYPE>
    CRBSplineParser2<TYPE>::~CRBSplineParser2()
    {
    }

    template<typename TYPE>
    void CRBSplineParser2<TYPE>::SetNurbsNode(const NurbsNode<TYPE>* pNurbsNode_, bool bRational_)
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

        // Construct the poles with weights.
        m_vecWeightedPoles.clear();
        for (size_t i = 0, _nSize = m_pNurbsNode->vecPoles.size(); i < _nSize; ++i) {
            m_vecWeightedPoles.push_back(m_pNurbsNode->vecPoles[i] * m_pNurbsNode->vecWeights[i]);
        }

        RecordValidPoles(m_nKnotIndex);
    }

    template<typename TYPE>
    void CRBSplineParser2<TYPE>::GetNurbsDeriv(double nKnot_, TYPE& ptPoint_, TYPE& ptDeriv1_, TYPE& ptDeriv2_)
    {
        NEAT_RAISE_IF(m_pNurbsNode->nDegree < 2, g_strWrongNurbsDegree);
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

        DeBoorAlgorithm(nKnot_, _nCurKnotIndex, 0, _nDegree - 2);

        double _nAlpha1 = _nDegree / (_vecKnots[_nCurKnotIndex + 1] - _vecKnots[_nCurKnotIndex]);
        double _nAlpha2 = 1.0 / (_vecKnots[_nCurKnotIndex + 2] - _vecKnots[_nCurKnotIndex]);
        double _nAlpha3 = 1.0 / (_vecKnots[_nCurKnotIndex + 1] - _vecKnots[_nCurKnotIndex - 1]);
        m_nxNumeratorDeriv[2] = _nAlpha1 * (_nDegree - 1) * ((m_vecTempPoles1[_nDegree] - m_vecTempPoles1[_nDegree - 1]) * _nAlpha2
            - (m_vecTempPoles1[_nDegree - 1] - m_vecTempPoles1[_nDegree - 2]) * _nAlpha3);
        m_nxDenominatorDeriv[2] = _nAlpha1 * (_nDegree - 1) * ((m_vecTempWeights1[_nDegree] - m_vecTempWeights1[_nDegree - 1]) * _nAlpha2
            - (m_vecTempWeights1[_nDegree - 1] - m_vecTempWeights1[_nDegree - 2]) * _nAlpha3);

        DeBoorAlgorithm(nKnot_, _nCurKnotIndex, _nDegree - 2, _nDegree - 1);
        m_nxNumeratorDeriv[1] = _nAlpha1 * (m_vecTempPoles1[_nDegree] - m_vecTempPoles1[_nDegree - 1]);
        m_nxDenominatorDeriv[1] = _nAlpha1 * (m_vecTempWeights1[_nDegree] - m_vecTempWeights1[_nDegree - 1]);

        DeBoorAlgorithm(nKnot_, _nCurKnotIndex, _nDegree - 1, _nDegree);
        m_nxNumeratorDeriv[0] = m_vecTempPoles1[_nDegree];
        m_nxDenominatorDeriv[0] = m_vecTempWeights1[_nDegree];

        double _nAlpha = 1.0 / m_nxDenominatorDeriv[0];
        ptPoint_ = m_nxNumeratorDeriv[0] * _nAlpha;
        ptDeriv1_ = (m_nxNumeratorDeriv[1] * m_nxDenominatorDeriv[0] - m_nxNumeratorDeriv[0] * m_nxDenominatorDeriv[1]) * _nAlpha * _nAlpha;
        ptDeriv2_ = (m_nxNumeratorDeriv[2] * m_nxDenominatorDeriv[0] - 2.0 * ptDeriv1_ * m_nxDenominatorDeriv[0] * m_nxDenominatorDeriv[1]
            - m_nxNumeratorDeriv[0] * m_nxDenominatorDeriv[2]) * _nAlpha * _nAlpha;
    }

    template<typename TYPE>
    void CRBSplineParser2<TYPE>::RecordValidPoles(int nKnotIndex_)
    {
        NEAT_RAISE_IF(nKnotIndex_ < m_nStartIndex || nKnotIndex_ > m_nEndIndex, g_strNurbsParamsError);

        m_vecValidPoles.clear();
        for (int i = nKnotIndex_ - m_pNurbsNode->nDegree, _nSize = nKnotIndex_ + 1; i < _nSize; ++i) {
            m_vecValidPoles.push_back(m_vecWeightedPoles[i]);
        }

        m_vecValidWeights.clear();
        auto _iterWeightBegin = m_pNurbsNode->vecWeights.begin() + nKnotIndex_ - m_pNurbsNode->nDegree;
        auto _iterWeightEnd = m_pNurbsNode->vecWeights.begin() + nKnotIndex_ + 1;
        for (; _iterWeightBegin != _iterWeightEnd; ++_iterWeightBegin) {
            m_vecValidWeights.push_back(*_iterWeightBegin);
        }
    }

    template<typename TYPE>
    void CRBSplineParser2<TYPE>::DeBoorAlgorithm(double nKnot_, int nKnotIndex_, int nStartIndex_, int nEndIndex_)
    {
        NEAT_RAISE_IF(nStartIndex_ > nEndIndex_, g_strNurbsParamsError);

        const int& _nDegree = m_pNurbsNode->nDegree;
        const VECDOUBLE& _vecKnots = m_pNurbsNode->vecKnots;

        for (int i = nStartIndex_; i < nEndIndex_; ++i) {
            for (int j = i; j < _nDegree; ++j) {
                int _nTempIndex = nKnotIndex_ - _nDegree + j + 1;
                double _nAlpha = (nKnot_ - _vecKnots[_nTempIndex]) / (_vecKnots[_nDegree + _nTempIndex - i] - _vecKnots[_nTempIndex]);
                m_vecTempPoles2[j + 1] = m_vecTempPoles1[j] * (1.0 - _nAlpha) + m_vecTempPoles1[j + 1] * _nAlpha;
                m_vecTempWeights2[j + 1] = m_vecTempWeights1[j] * (1.0 - _nAlpha) + m_vecTempWeights1[j + 1] * _nAlpha;
            }

            for (int k = i + 1; k <= _nDegree; ++k) {
                m_vecTempPoles1[k] = m_vecTempPoles2[k];
                m_vecTempWeights1[k] = m_vecTempWeights2[k];
            }
        }
    }

} // End namespace neat

#endif // __RBSPLINE_PARSER2_H_20200130__