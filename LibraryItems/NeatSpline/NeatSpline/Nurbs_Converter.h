/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NURBS_CONVERTER_H_20200130__
#define __NURBS_CONVERTER_H_20200130__

/**
 * This file defines the converter for nurbs node.
 */
namespace neat
{
    template<typename TYPE, typename TYPEEX>
    class CNurbsConverter
    {
    public:
        CNurbsConverter();
        ~CNurbsConverter();

        void ConvertNurbsToBezier(const NurbsNode<TYPE>* pNurbsNode_, std::vector<BezierNode<TYPE>>& vecBezierNodes_);

    private:
        CNurbsRefiner<TYPE, TYPEEX> m_NurbsRefiner;
        CFastVector<double> m_vecDivideKnots;
    };

    /**
     * The implementation of template class.
     */
    template<typename TYPE, typename TYPEEX>
    CNurbsConverter<TYPE, TYPEEX>::CNurbsConverter()
    {
    }

    template<typename TYPE, typename TYPEEX>
    CNurbsConverter<TYPE, TYPEEX>::~CNurbsConverter()
    {
    }

    template<typename TYPE, typename TYPEEX>
    void CNurbsConverter<TYPE, TYPEEX>::ConvertNurbsToBezier(const NurbsNode<TYPE>* pNurbsNode_, std::vector<BezierNode<TYPE>>& vecBezierNodes_)
    {
        // divide nurbs node in every knot
        bool _bRational = CheckNurbsNodeRational(pNurbsNode_);

        m_NurbsRefiner.SetNurbsNode(pNurbsNode_, _bRational);

        int _nSize = static_cast<int>(pNurbsNode_->vecKnots.size()) - pNurbsNode_->nDegree - 1;
        m_vecDivideKnots.resize(_nSize - pNurbsNode_->nDegree);
        for (int i = pNurbsNode_->nDegree, _nSize = static_cast<int>(pNurbsNode_->vecKnots.size()) - pNurbsNode_->nDegree - 1; i < _nSize; ++i) {
            m_vecDivideKnots[i - pNurbsNode_->nDegree] = pNurbsNode_->vecKnots[i];
        }

        CFastVector<NurbsNode<TYPE>>* _pvecNurbsNodes = m_NurbsRefiner.DivideNurbs(m_vecDivideKnots);

        // convert nurbs nodes to bezier nodes.
        vecBezierNodes_.clear();
        vecBezierNodes_.resize(_pvecNurbsNodes->size());
        for (int i = 0, _nSize = static_cast<int>(_pvecNurbsNodes->size()); i < _nSize; ++i) {
            NurbsNode<TYPE>& _nodeNurbs = (*_pvecNurbsNodes)[i];
            NEAT_RAISE_IF(static_cast<int>(_nodeNurbs.vecKnots.size()) < (_nodeNurbs.nDegree + 1) * 2, g_strNurbsParamsError);
            NEAT_RAISE_IF(static_cast<int>(_nodeNurbs.vecPoles.size()) < _nodeNurbs.nDegree + 1, g_strNurbsParamsError);

            // bezier node
            BezierNode<TYPE>& _nodeBezier = vecBezierNodes_[i];
            _nodeBezier.vecPoles.reserve(pNurbsNode_->nDegree + 1);
            _nodeBezier.vecWeights.reserve(pNurbsNode_->nDegree + 1);

            // bezier knots
            _nodeBezier.nStartKnot = _nodeNurbs.vecKnots.front();
            _nodeBezier.nEndKnot = _nodeNurbs.vecKnots.back();

            int _nStartIndex = 0;
            NEAT_RAISE_IF(!DOUBLE_EQU(_nodeNurbs.vecKnots[_nStartIndex], _nodeNurbs.vecKnots[_nStartIndex + _nodeNurbs.nDegree]), g_strNurbsParamsError);
            while (DOUBLE_EQU(_nodeNurbs.vecKnots[_nStartIndex], _nodeNurbs.vecKnots[_nStartIndex + _nodeNurbs.nDegree + 1])) {
                ++_nStartIndex;
            }

            // bezier poles
            NEAT_RAISE_IF(_nStartIndex + _nodeNurbs.nDegree >= static_cast<int>(_nodeNurbs.vecPoles.size()), g_strNurbsParamsError);
            for (int j = _nStartIndex; j <= _nStartIndex + _nodeNurbs.nDegree; ++j) {
                _nodeBezier.vecPoles.push_back(_nodeNurbs.vecPoles[j]);
            }

            // bezier weights
            int _nMaxIndex = min(_nStartIndex + _nodeNurbs.nDegree, static_cast<int>(_nodeNurbs.vecWeights.size()) - 1);
            for (int j = _nStartIndex; j <= _nMaxIndex; ++j) {
                _nodeBezier.vecWeights.push_back(_nodeNurbs.vecWeights[j]);
            }
        }
    }

} // End namespace neat

#endif // __NURBS_CONVERTER_H_20200130__