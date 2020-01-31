/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NURBS_SCATTER_H_20200130__
#define __NURBS_SCATTER_H_20200130__

/**
 * This file defines the scatter for nurbs node.
 */
namespace neat
{
    template<typename TYPE, typename TYPEEX>
    class CNurbsScatter
    {
    public:
        CNurbsScatter();
        ~CNurbsScatter();

        void ScatterNurbs(const NurbsNode<TYPE>* pNurbsNode_, double nDeflection_, std::vector<TYPE>& vecScatterPoints_);
        void ScatterNurbs(const NurbsNode<TYPE>* pNurbsNode_, double nDeflection_, std::vector<ScatterNode<TYPE>>& vecScatterNodes_);

    private:
        CFastVector<ScatterNode<TYPE>>* DeflectNurbs(const NurbsNode<TYPE>* pNurbsNode_, double nDeflection_);
        void CombineCollinearPoints(const CFastVector<ScatterNode<TYPE>>& vecScatterNodesIn_,
            CFastVector<ScatterNode<TYPE>>& vecScatterNodesOut_);
        bool JudgeCollinearPoints(const TYPE& ptPoint1_, const TYPE& ptPoint2_, const TYPE& ptPoint3_);

    private:
        CNurbsQuasiUniformDeflection<TYPE> m_NurbsQuasiUniformDeflection;
        CNurbsRefiner<TYPE, TYPEEX> m_NurbsRefiner;
        CFastVector<ScatterNode<TYPE>> m_vecScatterNodes;
        CFastVector<ScatterNode<TYPE>> m_vecTempScatterNodes;
        CFastVector<double> m_vecEmptyVector;
    };

    /**
     * The implementation of template class.
     */
    template<typename TYPE, typename TYPEEX>
    CNurbsScatter<TYPE, TYPEEX>::CNurbsScatter()
    {
    }

    template<typename TYPE, typename TYPEEX>
    CNurbsScatter<TYPE, TYPEEX>::~CNurbsScatter()
    {
    }

    template<typename TYPE, typename TYPEEX>
    void CNurbsScatter<TYPE, TYPEEX>::ScatterNurbs(const NurbsNode<TYPE>* pNurbsNode_, double nDeflection_, std::vector<TYPE>& vecScatterPoints_)
    {
        m_NurbsQuasiUniformDeflection.SetOnlyGetPoints(true);

        // discrete NURBS curve
        CFastVector<ScatterNode<TYPE>>* _pvecScatterNodes = DeflectNurbs(pNurbsNode_, nDeflection_);

        // output discrete points
        vecScatterPoints_.clear();
        vecScatterPoints_.reserve(_pvecScatterNodes->size());
        for (int i = 0, _nSize = static_cast<int>(_pvecScatterNodes->size()); i < _nSize; ++i) {
            vecScatterPoints_.push_back((*_pvecScatterNodes)[i].ptPoint);
        }
    }

    template<typename TYPE, typename TYPEEX>
    void CNurbsScatter<TYPE, TYPEEX>::ScatterNurbs(const NurbsNode<TYPE>* pNurbsNode_, double nDeflection_,
        std::vector<ScatterNode<TYPE>>& vecScatterNodes_)
    {
        NEAT_RAISE(pNurbsNode_->nDegree < 2, g_strWrongNurbsDegree);

        m_NurbsQuasiUniformDeflection.SetOnlyGetPoints(false);

        // discrete NURBS curve
        CFastVector<ScatterNode<TYPE>>* _pvecScatterNodes = DeflectNurbs(pNurbsNode_, nDeflection_);

        // output discrete nodes
        vecScatterNodes_.clear();
        vecScatterNodes_.reserve(_pvecScatterNodes->size());
        for (int i = 0, _nSize = static_cast<int>(_pvecScatterNodes->size()); i < _nSize; ++i) {
            vecScatterNodes_.push_back((*_pvecScatterNodes)[i]);
        }
    }

    template<typename TYPE, typename TYPEEX>
    CFastVector<ScatterNode<TYPE>>* CNurbsScatter<TYPE, TYPEEX>::DeflectNurbs(const NurbsNode<TYPE>* pNurbsNode_, double nDeflection_)
    {
        // divide nurbs node in repeat knots
        bool _bRational = CheckNurbsNodeRational(pNurbsNode_);
        m_NurbsRefiner.SetNurbsNode(pNurbsNode_, _bRational);

        m_vecEmptyVector.clear();
        CFastVector<NurbsNode<TYPE>>* _pvecNurbsNodes = m_NurbsRefiner.DivideNurbs(m_vecEmptyVector);

        // scatter nurbs node by given deflection
        m_vecScatterNodes.clear();
        m_vecTempScatterNodes.clear();
        m_vecTempScatterNodes.push_back(ScatterNode<TYPE>());

        for (int i = 0, _nSize = static_cast<int>(_pvecNurbsNodes->size()); i < _nSize; ++i) {
            m_NurbsQuasiUniformDeflection.ScatterNurbs(&((*_pvecNurbsNodes)[i]), nDeflection_, m_vecScatterNodes);
            m_vecTempScatterNodes.pop_back();
            m_vecTempScatterNodes.copy(m_vecScatterNodes);
        }

        // combine collinear points
        CombineCollinearPoints(m_vecTempScatterNodes, m_vecScatterNodes);

        return &m_vecScatterNodes;
    }

    template<typename TYPE, typename TYPEEX>
    void CNurbsScatter<TYPE, TYPEEX>::CombineCollinearPoints(const CFastVector<ScatterNode<TYPE>>& vecScatterNodesIn_,
        CFastVector<ScatterNode<TYPE>>& vecScatterNodesOut_)
    {
        vecScatterNodesOut_.clear();

        const int _nSize = static_cast<int>(vecScatterNodesIn_.size());
        if (0 == _nSize) {
            return;
        }

        // start point
        vecScatterNodesOut_.push_back(vecScatterNodesIn_.front());
        int _nIndex = 0;
        while (++_nIndex < _nSize) {
            double _nLength = (vecScatterNodesIn_[_nIndex].ptPoint - vecScatterNodesOut_.back().ptPoint).GetLength();
            if (!DOUBLE_EQU_ZERO(_nLength)) {
                vecScatterNodesOut_.push_back(vecScatterNodesIn_[_nIndex]);
                break;
            }
        }

        if (_nIndex + 1 >= _nSize) {
            return;
        }

        // other points
        for (int i = _nIndex + 1; i < _nSize; ++i) {
            int _nOutSize = static_cast<int>(vecScatterNodesOut_.size());
            const TYPE& _ptPrePoint = vecScatterNodesOut_[_nOutSize - 2].ptPoint;
            const TYPE& _ptCurPoint = vecScatterNodesOut_[_nOutSize - 1].ptPoint;
            const TYPE& _ptNextPoint = vecScatterNodesIn_[i].ptPoint;
            if (JudgeCollinearPoints(_ptPrePoint, _ptCurPoint, _ptNextPoint)) {
                vecScatterNodesOut_.back() = vecScatterNodesIn_[i];
            }
            else {
                vecScatterNodesOut_.push_back(vecScatterNodesIn_[i]);
            }
        }
    }

    template<typename TYPE, typename TYPEEX>
    bool CNurbsScatter<TYPE, TYPEEX>::JudgeCollinearPoints(const TYPE& ptPoint1_, const TYPE& ptPoint2_, const TYPE& ptPoint3_)
    {
        double _nLength12 = (ptPoint2_ - ptPoint1_).GetLength();
        double _nLength23 = (ptPoint3_ - ptPoint2_).GetLength();
        double _nLength13 = (ptPoint3_ - ptPoint1_).GetLength();
        return DOUBLE_EQU_ZERO(_nLength12 + _nLength23 - _nLength13);
    }

} // End namespace neat

#endif // __NURBS_SCATTER_H_20200130__