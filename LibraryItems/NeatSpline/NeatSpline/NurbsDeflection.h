/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NURBS_DEFLECTION_H_20200130__
#define __NURBS_DEFLECTION_H_20200130__

#include <list>

/**
 * This file defines the deflection for Non-uniform rational B-spline curve.
 */
namespace neat
{
    /**
     * The quasi uniform deflection for nurbs node
     */
    template<typename TYPE>
    class CNurbsQuasiUniformDeflection
    {
    public:
        CNurbsQuasiUniformDeflection();
        ~CNurbsQuasiUniformDeflection();

        // If you only get the point coordinates, you don't need to calculate the second-order guide of the NURBS curve.
        void SetOnlyGetPoints(bool bOnlyGetPoints_) { m_bOnlyGetPoints = bOnlyGetPoints_; }

        // Scatter nurbs node by given deflection, and output the scatter nodes.
        void ScatterNurbs(const NurbsNode<TYPE>* pNurbsNode_, double nDeflection_,
            CFastVector<ScatterNode<TYPE>>& vecScatterNodes_);

    private:
        // Get scatter node with given knot.
        void GetScatterNode(double nKnot_, ScatterNode<TYPE>& ScatterNode_);

        // Quasi uniform scatter nurbs node between the given scope.
        void QuasiFlecheNurbs(double nDeflection_, double nStartKnot_, double nEndKnot_);

    private:
        CNurbsParser<TYPE> m_NurbsParser;
        CFastVector<ScatterNode<TYPE>>* m_pvecScatterNodes;

        // temporary variable reutilization
        TYPE m_nxTempPoints[4];
        bool m_bOnlyGetPoints;
    };

    /**
     * The implementation of template class.
     */
    template<typename TYPE>
    CNurbsQuasiUniformDeflection<TYPE>::CNurbsQuasiUniformDeflection()
    {
    }

    template<typename TYPE>
    CNurbsQuasiUniformDeflection<TYPE>::~CNurbsQuasiUniformDeflection()
    {
    }

    template<typename TYPE>
    void CNurbsQuasiUniformDeflection<TYPE>::ScatterNurbs(const NurbsNode<TYPE>* pNurbsNode_, double nDeflection_,
        CFastVector<ScatterNode<TYPE>>& vecScatterNodes_)
    {
        NEAT_RAISE_IF(nDeflection_ <= 0.0, g_strWrongDeflection);
        m_NurbsParser.SetNurbsNode(pNurbsNode_);

        // check repeat knot at front and back
        NEAT_RAISE_IF(!DOUBLE_EQU(pNurbsNode_->vecKnots[pNurbsNode_->nDegree], pNurbsNode_->vecKnots.front())
            || !DOUBLE_EQU(pNurbsNode_->vecKnots[pNurbsNode_->vecKnots.size() - pNurbsNode_->nDegree - 1], pNurbsNode_->vecKnots.back()),
            g_strNurbsParamsError);

        m_pvecScatterNodes = &vecScatterNodes_;
        m_pvecScatterNodes->clear();
        m_pvecScatterNodes->push_back(ScatterNode<TYPE>());

        // collect the no repeat knots
        CFastVector<double> _vecValidKnots;
        _vecValidKnots.push_back(pNurbsNode_->vecKnots[pNurbsNode_->nDegree]);
        for (int i = pNurbsNode_->nDegree, _nSize = static_cast<int>(pNurbsNode_->vecKnots.size()) - pNurbsNode_->nDegree - 1; i <= _nSize; ++i) {
            if (!DOUBLE_EQU(pNurbsNode_->vecKnots[i], _vecValidKnots.back())) {
                _vecValidKnots.push_back(pNurbsNode_->vecKnots[i]);
            }
        }

        // Insert internal knots to avoid periodic or symmetrical control points
        int _nInnerCount = max(2, pNurbsNode_->nDegree - 1);

        // Adjust the number internal knots to even number
        if (_nInnerCount % 2 != 0)
        {
            _nInnerCount += 1;
        }

        for (int i = 1, _nSize = static_cast<int>(_vecValidKnots.size()); i < _nSize; ++i) {
            double _nStartKnot = _vecValidKnots[i - 1];
            for (int j = 1; j < _nInnerCount; ++j)
            {
                double _nRatio = 1.0 * j / _nInnerCount;
                double _nEndKnot = (1.0 - _nRatio) * _vecValidKnots[i - 1] + _nRatio * _vecValidKnots[i];
                QuasiFlecheNurbs(nDeflection_, _nStartKnot, _nEndKnot);
                _nStartKnot = _nEndKnot;
            }

            QuasiFlecheNurbs(nDeflection_, _nStartKnot, _vecValidKnots[i]);
        }
    }

    template<typename TYPE>
    void CNurbsQuasiUniformDeflection<TYPE>::GetScatterNode(double nKnot_, ScatterNode<TYPE>& ScatterNode_)
    {
        ScatterNode_.nKnot = nKnot_;
        if (m_bOnlyGetPoints) {
            m_NurbsParser.GetNurbsDeriv(nKnot_, ScatterNode_.ptPoint, ScatterNode_.ptDeriv1);
        }
        else {
            m_NurbsParser.GetNurbsDeriv(nKnot_, ScatterNode_.ptPoint, ScatterNode_.ptDeriv1, ScatterNode_.ptDeriv2);
        }
    }

    template<typename TYPE>
    void CNurbsQuasiUniformDeflection<TYPE>::QuasiFlecheNurbs(double nDeflection_, double nStartKnot_, double nEndKnot_)
    {
        ScatterNode<TYPE> _StartScatterNode, _EndScatterNode;
        GetScatterNode(nStartKnot_, _StartScatterNode);
        GetScatterNode(nEndKnot_, _EndScatterNode);

        m_pvecScatterNodes->pop_back();
        m_pvecScatterNodes->push_back(_StartScatterNode);

        const double _nSquareTolerance = Precision::SquareRealTolerance();
        const double _nSquareDeflection = nDeflection_ * nDeflection_;
        const int _nPointMin = 2;

        // Unprocessed nodes
        struct UndoNodes
        {
            ScatterNode<TYPE> node;
            int nPointMin = 0;
        };

        std::list<UndoNodes> _listUndoNodes;
        _listUndoNodes.emplace_front(UndoNodes{ _EndScatterNode, _nPointMin });

        // recursion could stack overflow, loop instead
        ScatterNode<TYPE> _NextKnot;
        while (!_listUndoNodes.empty()) {
            ScatterNode<TYPE>& _StartKnot = m_pvecScatterNodes->back();
            UndoNodes& _UndoEndKnot = _listUndoNodes.front();

            int _nPointSize = static_cast<int>(m_pvecScatterNodes->size());
            double _nDeltaKnot = _UndoEndKnot.node.nKnot - _StartKnot.nKnot;
            _NextKnot = _UndoEndKnot.node;
            if (_UndoEndKnot.nPointMin > 2) {
                _nDeltaKnot /= (_UndoEndKnot.nPointMin - 1);
                GetScatterNode(_StartKnot.nKnot + _nDeltaKnot, _NextKnot);
            }

            double _nNorme = (_NextKnot.ptPoint - _StartKnot.ptPoint).GetSquareLength();
            double _nTheFleche = 0.0;
            bool _bFlecheOK = false;
            if (_nNorme > _nSquareTolerance) {
                double _nNorme1 = _StartKnot.ptDeriv1.GetSquareLength();
                double _nNorme2 = _NextKnot.ptDeriv1.GetSquareLength();
                if (_nNorme1 > _nSquareTolerance && _nNorme2 > _nSquareTolerance) {
                    m_nxTempPoints[0] = _StartKnot.ptDeriv1 / _StartKnot.ptDeriv1.GetLength();
                    m_nxTempPoints[1] = _NextKnot.ptDeriv1 / _NextKnot.ptDeriv1.GetLength();
                    double _nNormeDiff = (m_nxTempPoints[0] - m_nxTempPoints[1]).GetSquareLength();
                    if (_nNormeDiff > _nSquareTolerance) {
                        _nTheFleche = _nNormeDiff * _nNorme / 64.0;
                        _bFlecheOK = true;
                    }
                }
            }

            if (!_bFlecheOK) {
                m_nxTempPoints[2] = (_StartKnot.ptPoint + _NextKnot.ptPoint) * 0.5;
                m_NurbsParser.GetNurbsPoint(_StartKnot.nKnot + 0.5 * _nDeltaKnot, m_nxTempPoints[3]);
                _nTheFleche = (m_nxTempPoints[2] - m_nxTempPoints[3]).GetSquareLength();
            }

            if (_nTheFleche < _nSquareDeflection) {
                m_pvecScatterNodes->push_back(_NextKnot);
            }
            else {
                _listUndoNodes.emplace_front(UndoNodes{ _NextKnot, 3 });
                continue;
            }

            if (_UndoEndKnot.nPointMin > 2) {
                _UndoEndKnot.nPointMin += _nPointSize - static_cast<int>(m_pvecScatterNodes->size());
            }
            else {
                _listUndoNodes.pop_front();
            }
        }
    }

} // End namespace neat

#endif // __NURBS_DEFLECTION_H_20200130__