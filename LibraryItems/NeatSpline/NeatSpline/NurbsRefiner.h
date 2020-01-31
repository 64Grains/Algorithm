/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NURBS_REFINER_H_20200130__
#define __NURBS_REFINER_H_20200130__

/**
 * This file defines the refiner for Non-uniform rational B-spline curve.
 */
namespace neat
{
    /**
     * The refiner for nurbs node
     */
    template<typename TYPE, typename TYPEEX>
    class CNurbsRefiner
    {
    public:
        CNurbsRefiner();
        ~CNurbsRefiner();

        // Set the information for nurbs node.
        void SetNurbsNode(const NurbsNode<TYPE>* pNurbsNode_, bool bRational_);

        // Divide nurbs node in the given knots and repeated knots, and return nurbs node subsection.
        CFastVector<NurbsNode<TYPE>>* DivideNurbs(const CFastVector<double>& vecDivideKnots_);

    private:
        // Construct repeated knots.
        void ConstructRepeatedKnots(const CFastVector<double>& vecDivideKnots_);

        // Divide non rational BSpline in the repeated knots.
        void DivideNRBSpline(const CFastVector<double>& vecRepeatedKnots_);

        // Divide rational BSpline in the repeated knots.
        void DivideRBSpline(const CFastVector<double>& vecRepeatedKnots_);

        // Get nurbs node subsection in the repeated knots.
        void GetNurbsSubsection(const NurbsNode<TYPE>* pNurbsNode_);

        // Conversion of control point coordinates and high-dimensional homogeneous coordinates
        void UpgradePoles(const TYPE& ptPole_, double nWeight_, TYPEEX& ptHomogeneousPole_) const;
        void DowngradePoles(const TYPEEX& ptHomogeneousPole_, TYPE& ptPole_, double& nWeight_) const;

    private:
        NurbsNode<TYPE>* m_pNurbsNode;
        int m_nStartIndex;
        int m_nEndIndex;
        int m_nKnotIndex;
        bool m_bRational;

        // The nurbs node subsection
        CFastVector<NurbsNode<TYPE>> m_vecNurbsNodes;

        // The refined nurbs node
        NurbsNode<TYPE> m_RefinedNurbsNode;

        // High dimensional homogeneous coordinates.
        CFastVector<TYPEEX> m_vecSrcPoles;
        CFastVector<TYPEEX> m_vecDestPoles;

        // The repeated knots.
        CFastVector<double> m_vecRepeatedKnots;
    };

    /**
     * The implementation of template class.
     */
    template<typename TYPE, typename TYPEEX>
    CNurbsRefiner<TYPE, TYPEEX>::CNurbsRefiner()
        : m_pNurbsNode(nullptr)
        , m_nStartIndex(0)
        , m_nEndIndex(0)
        , m_nKnotIndex(0)
        , m_bRational(false)
    {
    }

    template<typename TYPE, typename TYPEEX>
    CNurbsRefiner<TYPE, TYPEEX>::~CNurbsRefiner()
    {
    }

    template<typename TYPE, typename TYPEEX>
    void CNurbsRefiner<TYPE, TYPEEX>::SetNurbsNode(const NurbsNode<TYPE>* pNurbsNode_, bool bRational_)
    {
        NEAT_RAISE_IF(pNurbsNode_ == nullptr, g_strEmptyPointer);
        NEAT_RAISE_IF(CheckNurbsNodeRational(pNurbsNode_) != bRational_, g_strNurbsParamsError);

        m_pNurbsNode = (NurbsNode<TYPE>*)pNurbsNode_;
        m_bRational = bRational_;

        // Initialize the knot index limit value
        m_nStartIndex = m_pNurbsNode->nDegree;
        m_nEndIndex = static_cast<int>(m_pNurbsNode->vecKnots.size()) - m_pNurbsNode->nDegree - 1;
        NEAT_RAISE(DOUBLE_EQU(m_pNurbsNode->vecKnots[m_nStartIndex], m_pNurbsNode->vecKnots[m_nEndIndex]), g_strNurbsParamsError);

        m_nKnotIndex = m_nStartIndex;
    }

    template<typename TYPE, typename TYPEEX>
    CFastVector<NurbsNode<TYPE>>* CNurbsRefiner<TYPE, TYPEEX>::DivideNurbs(const CFastVector<double>& vecDivideKnots_)
    {
        NEAT_RAISE_IF(m_pNurbsNode == nullptr, g_strEmptyPointer);

        m_vecNurbsNodes.clear();

        // Construct repeated knots.
        ConstructRepeatedKnots(vecDivideKnots_);

        // No repeated knots.
        if (m_vecRepeatedKnots.empty()) {
            GetNurbsSubsection(m_pNurbsNode);
            return &m_vecNurbsNodes;
        }

        NEAT_RAISE_IF(m_vecRepeatedKnots.front() < m_pNurbsNode->vecKnots[m_nStartIndex], g_strNurbsParamsError);
        NEAT_RAISE_IF(m_vecRepeatedKnots.back() > m_pNurbsNode->vecKnots[m_nEndIndex], g_strNurbsParamsError);
        if (m_bRational) {
            DivideRBSpline(m_vecRepeatedKnots);
        }
        else {
            DivideNRBSpline(m_vecRepeatedKnots);
        }

        return &m_vecNurbsNodes;
    }

    template<typename TYPE, typename TYPEEX>
    void CNurbsRefiner<TYPE, TYPEEX>::ConstructRepeatedKnots(const CFastVector<double>& vecDivideKnots_)
    {
        // check divide knots
        for (int i = 1, _nSize = static_cast<int>(vecDivideKnots_.size()); i < _nSize; ++i) {
            NEAT_RAISE_IF(vecDivideKnots_[i] < vecDivideKnots_[i - 1], g_strNurbsParamsError);
        }

        if (!vecDivideKnots_.empty()) {
            NEAT_RAISE_IF(vecDivideKnots_.front() < m_pNurbsNode->vecKnots[m_nStartIndex]
                || vecDivideKnots_.back() > m_pNurbsNode->vecKnots[m_nEndIndex], g_strNurbsParamsError);
        }

        // find the index of start knot and end knot
        int _nStartIndex = 0;
        while (true) {
            if (DOUBLE_EQU(m_pNurbsNode->vecKnots[_nStartIndex], m_pNurbsNode->vecKnots[m_nStartIndex])) {
                break;
            }

            ++_nStartIndex;
        }

        int _nEndIndex = static_cast<int>(m_pNurbsNode->vecKnots.size()) - 1;
        while (true) {
            if (DOUBLE_EQU(m_pNurbsNode->vecKnots[_nEndIndex], m_pNurbsNode->vecKnots[m_nEndIndex])) {
                break;
            }

            --_nEndIndex;
        }

        // start knot
        m_vecRepeatedKnots.clear();
        int _nRepeatCount = 1;
        for (int i = _nStartIndex + 1; i <= _nEndIndex; ++i) {
            if (!DOUBLE_EQU(m_pNurbsNode->vecKnots[i], m_pNurbsNode->vecKnots[_nStartIndex])) {
                break;
            }

            ++_nRepeatCount;
        }

        for (int i = 0, _nInsertCount = m_pNurbsNode->nDegree + 1 - _nRepeatCount; i < _nInsertCount; ++i) {
            m_vecRepeatedKnots.push_back(m_pNurbsNode->vecKnots[_nStartIndex]);
        }

        int _nDivideIndex = 0;
        int _nDivideCount = static_cast<int>(vecDivideKnots_.size());
        for (; _nDivideIndex < _nDivideCount; ++_nDivideIndex) {
            if (!DOUBLE_EQU(vecDivideKnots_[_nDivideIndex], m_pNurbsNode->vecKnots[_nStartIndex])) {
                break;
            }
        }

        // inner knots
        _nStartIndex += _nRepeatCount;
        _nRepeatCount = 1;
        double _nFlagKnot = m_pNurbsNode->vecKnots[_nStartIndex];
        for (int i = _nStartIndex + 1; i <= _nEndIndex; ++i) {
            if (DOUBLE_EQU(m_pNurbsNode->vecKnots[i], _nFlagKnot)) {
                ++_nRepeatCount;
                continue;
            }

            bool _bMustInsert = false;
            while (_nDivideIndex < _nDivideCount && DOUBLE_EQU(vecDivideKnots_[_nDivideIndex], _nFlagKnot)) {
                _bMustInsert = true;
                ++_nDivideIndex;
            }

            if (_nDivideIndex < _nDivideCount && vecDivideKnots_[_nDivideIndex] < _nFlagKnot) {
                double _nAimKnot = vecDivideKnots_[_nDivideIndex++];
                while (_nDivideIndex < _nDivideCount && DOUBLE_EQU(vecDivideKnots_[_nDivideIndex], _nAimKnot)) {
                    ++_nDivideIndex;
                }

                // insert repeat knot
                for (int j = 0; j < m_pNurbsNode->nDegree; ++j) {
                    m_vecRepeatedKnots.push_back(_nAimKnot);
                }
            }

            if (_nRepeatCount > 1 || m_pNurbsNode->nDegree == 1 || _bMustInsert) {
                // insert repeat knot
                for (int j = 0, _nInsertCount = m_pNurbsNode->nDegree + 1 - _nRepeatCount; j < _nInsertCount; ++j) {
                    m_vecRepeatedKnots.push_back(_nFlagKnot);
                }
            }

            _nFlagKnot = m_pNurbsNode->vecKnots[i];
            _nRepeatCount = 1;
        }

        if (_nRepeatCount > 1) {
            for (int j = 0, _nInsertCount = m_pNurbsNode->nDegree + 1 - _nRepeatCount; j < _nInsertCount; ++j) {
                m_vecRepeatedKnots.push_back(_nFlagKnot);
            }
        }

        // end knot
        if (m_vecRepeatedKnots.empty() || !DOUBLE_EQU(m_vecRepeatedKnots.back(), m_pNurbsNode->vecKnots[_nEndIndex])) {
            _nRepeatCount = 1;
            for (int i = _nEndIndex - 1; i >= m_nStartIndex; --i) {
                if (!DOUBLE_EQU(m_pNurbsNode->vecKnots[i], m_pNurbsNode->vecKnots[_nEndIndex])) {
                    break;
                }

                ++_nRepeatCount;
            }

            for (int i = 0, _nInsertCount = m_pNurbsNode->nDegree + 1 - _nRepeatCount; i < _nInsertCount; ++i) {
                m_vecRepeatedKnots.push_back(m_pNurbsNode->vecKnots[_nEndIndex]);
            }
        }
    }

    template<typename TYPE, typename TYPEEX>
    void CNurbsRefiner<TYPE, TYPEEX>::DivideNRBSpline(const CFastVector<double>& vecRepeatedKnots_)
    {
        // the degree of nurbs node
        m_RefinedNurbsNode.nDegree = m_pNurbsNode->nDegree;

        // the index of knot
        int _nRepeatSize = static_cast<int>(vecRepeatedKnots_.size());
        NEAT_RAISE_IF(_nRepeatSize == 0, g_strNurbsParamsError);

        int _nMinIndex = FindSpanForKnot(m_pNurbsNode->vecKnots, vecRepeatedKnots_.front(), m_nStartIndex, m_nEndIndex, m_nKnotIndex);
        int _nMaxIndex = FindSpanForKnot(m_pNurbsNode->vecKnots, vecRepeatedKnots_.back(), m_nStartIndex, m_nEndIndex, m_nKnotIndex) + 1;

        // copy the constant knots
        m_RefinedNurbsNode.vecKnots.resize(m_pNurbsNode->vecKnots.size() + _nRepeatSize);
        for (int i = 0; i <= _nMinIndex; ++i) {
            m_RefinedNurbsNode.vecKnots[i] = m_pNurbsNode->vecKnots[i];
        }

        for (int i = _nMaxIndex, _nSize = static_cast<int>(m_pNurbsNode->vecKnots.size()); i < _nSize; ++i) {
            m_RefinedNurbsNode.vecKnots[i + _nRepeatSize] = m_pNurbsNode->vecKnots[i];
        }

        // copy the constant poles
        m_RefinedNurbsNode.vecPoles.resize(m_pNurbsNode->vecPoles.size() + _nRepeatSize);
        for (int i = 0, _nSize = _nMinIndex - m_pNurbsNode->nDegree; i <= _nSize; ++i) {
            m_RefinedNurbsNode.vecPoles[i] = m_pNurbsNode->vecPoles[i];
        }

        for (int i = _nMaxIndex - 1, _nSize = static_cast<int>(m_pNurbsNode->vecPoles.size()); i < _nSize; ++i) {
            m_RefinedNurbsNode.vecPoles[i + _nRepeatSize] = m_pNurbsNode->vecPoles[i];
        }

        // new knots and poles
        int _nIndex = _nMaxIndex + m_pNurbsNode->nDegree;
        int _nFlag = _nMaxIndex + m_pNurbsNode->nDegree + _nRepeatSize;
        for (int j = _nRepeatSize - 1; j >= 0; --j) {
            while (vecRepeatedKnots_[j] <= m_pNurbsNode->vecKnots[_nIndex] && _nIndex > _nMinIndex) {
                m_RefinedNurbsNode.vecPoles[_nFlag - m_pNurbsNode->nDegree - 1] = m_pNurbsNode->vecPoles[_nIndex - m_pNurbsNode->nDegree - 1];
                m_RefinedNurbsNode.vecKnots[_nFlag] = m_pNurbsNode->vecKnots[_nIndex];
                --_nFlag;
                --_nIndex;
            }

            m_RefinedNurbsNode.vecPoles[_nFlag - m_pNurbsNode->nDegree - 1] = m_RefinedNurbsNode.vecPoles[_nFlag - m_pNurbsNode->nDegree];
            for (int k = 1; k <= m_pNurbsNode->nDegree; ++k) {
                int _nTemp = _nFlag - m_pNurbsNode->nDegree + k;
                double _nAlpha = m_RefinedNurbsNode.vecKnots[_nFlag + k] - vecRepeatedKnots_[j];
                if (DOUBLE_EQU_ZERO(_nAlpha)) {
                    m_RefinedNurbsNode.vecPoles[_nTemp - 1] = m_RefinedNurbsNode.vecPoles[_nTemp];
                }
                else {
                    _nAlpha /= (m_RefinedNurbsNode.vecKnots[_nFlag + k] - m_pNurbsNode->vecKnots[_nIndex - m_pNurbsNode->nDegree + k]);
                    m_RefinedNurbsNode.vecPoles[_nTemp - 1] = _nAlpha * m_RefinedNurbsNode.vecPoles[_nTemp - 1] + (1.0 - _nAlpha)
                        * m_RefinedNurbsNode.vecPoles[_nTemp];
                }
            }

            m_RefinedNurbsNode.vecKnots[_nFlag] = vecRepeatedKnots_[j];
            --_nFlag;
        }

        // Get nurbs node subsection
        GetNurbsSubsection(&m_RefinedNurbsNode);
    }

    template<typename TYPE, typename TYPEEX>
    void CNurbsRefiner<TYPE, TYPEEX>::DivideRBSpline(const CFastVector<double>& vecRepeatedKnots_)
    {
        // Translate three space coordinates to four dimensional homogeneous coordinates
        m_vecSrcPoles.resize(m_pNurbsNode->vecPoles.size());
        for (int i = 0, _nSize = static_cast<int>(m_pNurbsNode->vecPoles.size()); i < _nSize; ++i) {
            UpgradePoles(m_pNurbsNode->vecPoles[i], m_pNurbsNode->vecWeights[i], m_vecSrcPoles[i]);
        }

        // the degree of nurbs node
        m_RefinedNurbsNode.nDegree = m_pNurbsNode->nDegree;

        // the index of knot
        int _nRepeatSize = static_cast<int>(vecRepeatedKnots_.size());
        NEAT_RAISE_IF(_nRepeatSize == 0, g_strNurbsParamsError);

        int _nMinIndex = FindSpanForKnot(m_pNurbsNode->vecKnots, vecRepeatedKnots_.front(), m_nStartIndex, m_nEndIndex, m_nKnotIndex);
        int _nMaxIndex = FindSpanForKnot(m_pNurbsNode->vecKnots, vecRepeatedKnots_.back(), m_nStartIndex, m_nEndIndex, m_nKnotIndex) + 1;

        // copy the constant knots
        m_RefinedNurbsNode.vecKnots.resize(m_pNurbsNode->vecKnots.size() + _nRepeatSize);
        for (int i = 0; i <= _nMinIndex; ++i) {
            m_RefinedNurbsNode.vecKnots[i] = m_pNurbsNode->vecKnots[i];
        }

        for (int i = _nMaxIndex, _nSize = static_cast<int>(m_pNurbsNode->vecKnots.size()); i < _nSize; ++i) {
            m_RefinedNurbsNode.vecKnots[i + _nRepeatSize] = m_pNurbsNode->vecKnots[i];
        }

        // copy the constant poles
        m_vecDestPoles.resize(m_pNurbsNode->vecPoles.size() + _nRepeatSize);
        for (int i = 0, _nSize = _nMinIndex - m_pNurbsNode->nDegree; i <= _nSize; ++i) {
            m_vecDestPoles[i] = m_vecSrcPoles[i];
        }

        for (int i = _nMaxIndex - 1, _nSize = static_cast<int>(m_pNurbsNode->vecPoles.size()); i < _nSize; ++i) {
            m_vecDestPoles[i + _nRepeatSize] = m_vecSrcPoles[i];
        }

        // new knots and poles
        int _nIndex = _nMaxIndex + m_pNurbsNode->nDegree;
        int _nFlag = _nMaxIndex + m_pNurbsNode->nDegree + _nRepeatSize;
        for (int j = (int)_nRepeatSize - 1; j >= 0; --j) {
            while (vecRepeatedKnots_[j] <= m_pNurbsNode->vecKnots[_nIndex] && _nIndex > _nMinIndex) {
                m_vecDestPoles[_nFlag - m_pNurbsNode->nDegree - 1] = m_vecSrcPoles[_nIndex - m_pNurbsNode->nDegree - 1];
                m_RefinedNurbsNode.vecKnots[_nFlag] = m_pNurbsNode->vecKnots[_nIndex];
                --_nFlag;
                --_nIndex;
            }

            m_vecDestPoles[_nFlag - m_pNurbsNode->nDegree - 1] = m_vecDestPoles[_nFlag - m_pNurbsNode->nDegree];
            for (int k = 1; k <= m_pNurbsNode->nDegree; ++k) {
                int _nTemp = _nFlag - m_pNurbsNode->nDegree + k;
                double _nAlpha = m_RefinedNurbsNode.vecKnots[_nFlag + k] - vecRepeatedKnots_[j];
                if (DOUBLE_EQU_ZERO(_nAlpha)) {
                    m_vecDestPoles[_nTemp - 1] = m_vecDestPoles[_nTemp];
                }
                else {
                    _nAlpha /= (m_RefinedNurbsNode.vecKnots[_nFlag + k] - m_pNurbsNode->vecKnots[_nIndex - m_pNurbsNode->nDegree + k]);
                    m_vecDestPoles[_nTemp - 1] = _nAlpha * m_vecDestPoles[_nTemp - 1] + (1.0 - _nAlpha) * m_vecDestPoles[_nTemp];
                }
            }

            m_RefinedNurbsNode.vecKnots[_nFlag] = vecRepeatedKnots_[j];
            --_nFlag;
        }

        // Translate four dimensional homogeneous coordinates to three space coordinates
        m_RefinedNurbsNode.vecWeights.resize(m_vecDestPoles.size());
        m_RefinedNurbsNode.vecPoles.resize(m_vecDestPoles.size());
        for (int i = 0, _nSize = static_cast<int>(m_vecDestPoles.size()); i < _nSize; ++i) {
            DowngradePoles(m_vecDestPoles[i], m_RefinedNurbsNode.vecPoles[i], m_RefinedNurbsNode.vecWeights[i]);
        }

        // Get nurbs node subsection
        GetNurbsSubsection(&m_RefinedNurbsNode);
    }

    template<typename TYPE, typename TYPEEX>
    void CNurbsRefiner<TYPE, TYPEEX>::GetNurbsSubsection(const NurbsNode<TYPE>* pNurbsNode_)
    {
        // find the start index of repeated knot
        int _nStartFlagIndex = pNurbsNode_->nDegree;
        int _nStartIndex = 0;
        while (true) {
            if (DOUBLE_EQU(pNurbsNode_->vecKnots[_nStartIndex], pNurbsNode_->vecKnots[_nStartFlagIndex])) {
                break;
            }

            ++_nStartIndex;
        }

        // find the end index of repeated knot
        int _nEndFlagIndex = static_cast<int>(pNurbsNode_->vecKnots.size()) - pNurbsNode_->nDegree - 1;
        int _nEndIndex = static_cast<int>(pNurbsNode_->vecKnots.size()) - 1;
        while (true) {
            if (DOUBLE_EQU(pNurbsNode_->vecKnots[_nEndIndex], pNurbsNode_->vecKnots[_nEndFlagIndex])) {
                break;
            }

            --_nEndIndex;
        }

        // nurbs node subsection
        m_vecNurbsNodes.push_back(NurbsNode<TYPE>());
        m_vecNurbsNodes.back().nDegree = pNurbsNode_->nDegree;
        m_vecNurbsNodes.back().vecKnots.reserve(pNurbsNode_->nDegree * 2);

        int _nPointIndex = _nStartIndex;
        int _nRepeatCount = 1;
        double _nFlagKnot = pNurbsNode_->vecKnots[_nStartIndex];
        for (int i = _nStartIndex + 1; i <= _nEndIndex; ++i) {
            // repeated knot
            if (DOUBLE_EQU(pNurbsNode_->vecKnots[i], _nFlagKnot)) {
                ++_nRepeatCount;
                continue;
            }

            // no repeat inner knot
            if (_nRepeatCount == 1) {
                m_vecNurbsNodes.back().vecKnots.push_back(_nFlagKnot);
                _nFlagKnot = pNurbsNode_->vecKnots[i];
                continue;
            }

            // new knots
            NEAT_RAISE_IF(_nRepeatCount <= pNurbsNode_->nDegree, g_strNurbsParamsError);
            for (int j = 0; j < _nRepeatCount; ++j) {
                m_vecNurbsNodes.back().vecKnots.push_back(_nFlagKnot);
            }

            if (static_cast<int>(m_vecNurbsNodes.back().vecKnots.size()) <= pNurbsNode_->nDegree * 2) {
                _nFlagKnot = pNurbsNode_->vecKnots[i];
                _nRepeatCount = 1;
                continue;
            }

            // new poles
            int _nPointCount = static_cast<int>(m_vecNurbsNodes.back().vecKnots.size()) - pNurbsNode_->nDegree - 1;
            m_vecNurbsNodes.back().vecPoles.reserve(_nPointCount);
            for (int j = _nPointIndex, _nPointSize = _nPointIndex + _nPointCount; j < _nPointSize; ++j) {
                m_vecNurbsNodes.back().vecPoles.push_back(pNurbsNode_->vecPoles[j]);
            }

            // new weights
            if (m_bRational) {
                NEAT_RAISE_IF(pNurbsNode_->vecWeights.empty(), g_strNurbsParamsError);
                m_vecNurbsNodes.back().vecWeights.reserve(_nPointCount);
                for (int j = _nPointIndex, _nPointSize = _nPointIndex + _nPointCount; j < _nPointSize; ++j) {
                    m_vecNurbsNodes.back().vecWeights.push_back(pNurbsNode_->vecWeights[j]);
                }
            }

            // new nurb node
            m_vecNurbsNodes.push_back(NurbsNode<TYPE>());
            m_vecNurbsNodes.back().nDegree = pNurbsNode_->nDegree;
            m_vecNurbsNodes.back().vecKnots.reserve(pNurbsNode_->nDegree * 2);
            for (int j = 0; j < _nRepeatCount; ++j) {
                m_vecNurbsNodes.back().vecKnots.push_back(_nFlagKnot);
            }

            // reset flag value
            _nPointIndex += _nPointCount;
            _nFlagKnot = pNurbsNode_->vecKnots[i];
            _nRepeatCount = 1;
        }

        // last repeat knot
        NEAT_RAISE_IF(_nRepeatCount <= pNurbsNode_->nDegree, g_strNurbsParamsError);
        for (int j = 0; j < _nRepeatCount; ++j) {
            m_vecNurbsNodes.back().vecKnots.push_back(_nFlagKnot);
        }

        // last poles
        NEAT_RAISE_IF(_nPointIndex == pNurbsNode_->vecPoles.size(), g_strNurbsParamsError);
        int _nPointCount = static_cast<int>(m_vecNurbsNodes.back().vecKnots.size()) - pNurbsNode_->nDegree - 1;
        m_vecNurbsNodes.back().vecPoles.reserve(_nPointCount);
        for (int j = _nPointIndex, _nPointSize = _nPointIndex + _nPointCount; j < _nPointSize; ++j) {
            m_vecNurbsNodes.back().vecPoles.push_back(pNurbsNode_->vecPoles[j]);
        }

        // last weights
        if (m_bRational) {
            NEAT_RAISE_IF(pNurbsNode_->vecWeights.empty(), g_strNurbsParamsError);
            m_vecNurbsNodes.back().vecWeights.reserve(_nPointCount);
            for (int j = _nPointIndex, _nPointSize = _nPointIndex + _nPointCount; j < _nPointSize; ++j) {
                m_vecNurbsNodes.back().vecWeights.push_back(pNurbsNode_->vecWeights[j]);
            }
        }

        NEAT_RAISE_IF(_nPointIndex + _nPointCount != pNurbsNode_->vecPoles.size() - (pNurbsNode_->vecKnots.size() - 1 - _nEndIndex),
            g_strNurbsParamsError);
    }

} // End namespace neat

#endif // __NURBS_REFINER_H_20200130__