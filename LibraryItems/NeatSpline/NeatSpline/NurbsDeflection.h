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

        // Perform scatter nurbs node between the given scope.
        void PerformNurbs(double nDeflection_, double nStartKnot_, double nEndKnot_);

        // Quasi uniform scatter nurbs node recursively.
        void QuasiFleche(double nDeflection2_, const ScatterNode<TYPE>& StartNode_, const ScatterNode<TYPE>& EndNode_,
            int nPointMin_, double nEps_);

    private:
        CNurbsParser<TYPE> m_NurbsParser;
        CFastVector<double> m_vecValidKnots;
        ScatterNode<TYPE> m_StartScatterNode;
        ScatterNode<TYPE> m_EndScatterNode;
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
        m_vecValidKnots.clear();
        m_vecValidKnots.push_back(pNurbsNode_->vecKnots[pNurbsNode_->nDegree]);
        for (int i = pNurbsNode_->nDegree, _nSize = static_cast<int>(pNurbsNode_->vecKnots.size()) - pNurbsNode_->nDegree - 1; i <= _nSize; ++i) {
            if (!DOUBLE_EQU(pNurbsNode_->vecKnots[i], m_vecValidKnots.back())) {
                m_vecValidKnots.push_back(pNurbsNode_->vecKnots[i]);
            }
        }

        // select the middle knot between two no repeat knots
        for (int i = 1, _nSize = static_cast<int>(m_vecValidKnots.size()); i < _nSize; ++i) {
            double _nMiddleKnot = 0.5 * (m_vecValidKnots[i - 1] + m_vecValidKnots[i]);
            PerformNurbs(nDeflection_, m_vecValidKnots[i - 1], _nMiddleKnot);
            PerformNurbs(nDeflection_, _nMiddleKnot, m_vecValidKnots[i]);
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
    void CNurbsQuasiUniformDeflection<TYPE>::PerformNurbs(double nDeflection_, double nStartKnot_, double nEndKnot_)
    {
        int _nPointMin = 2;

        GetScatterNode(nStartKnot_, m_StartScatterNode);
        GetScatterNode(nEndKnot_, m_EndScatterNode);

        m_pvecScatterNodes->pop_back();
        m_pvecScatterNodes->push_back(m_StartScatterNode);

        const double _nTolerance2 = Precision::SquareRealTolerance();
        QuasiFleche(nDeflection_ * nDeflection_, m_StartScatterNode, m_EndScatterNode, _nPointMin, _nTolerance2);
    }

    template<typename TYPE>
    void CNurbsQuasiUniformDeflection<TYPE>::QuasiFleche(double nDeflection2_, const ScatterNode<TYPE>& StartNode_, const ScatterNode<TYPE>& EndNode_,
        int nPointMin_, double nEps_)
    {
        int _nPointSize = static_cast<int>(m_pvecScatterNodes->size());
        double _nDeltaKnot = EndNode_.nKnot - StartNode_.nKnot;
        ScatterNode<TYPE> _NextNode = EndNode_;
        if (nPointMin_ > 2) {
            _nDeltaKnot /= (nPointMin_ - 1);
            GetScatterNode(StartNode_.nKnot + _nDeltaKnot, _NextNode);
        }

        double _nNorme = (_NextNode.ptPoint - StartNode_.ptPoint).GetSquareLength();
        double _nTheFleche = 0.;
        bool _bFlecheOk = false;
        if (_nNorme > nEps_) {
            double _nNorme1 = StartNode_.ptDeriv1.GetSquareLength();
            double _nNorme2 = _NextNode.ptDeriv1.GetSquareLength();
            if (_nNorme1 > nEps_&& _nNorme2 > nEps_) {
                m_nxTempPoints[0] = StartNode_.ptDeriv1 / StartNode_.ptDeriv1.GetLength();
                m_nxTempPoints[1] = _NextNode.ptDeriv1 / _NextNode.ptDeriv1.GetLength();
                double _nNormeDiff = (m_nxTempPoints[0] - m_nxTempPoints[1]).GetSquareLength();
                if (_nNormeDiff > nEps_) {
                    _nTheFleche = _nNormeDiff * _nNorme / 64.0;
                    _bFlecheOk = true;
                }
            }
        }

        if (!_bFlecheOk) {
            m_nxTempPoints[2] = (StartNode_.ptPoint + _NextNode.ptPoint) * 0.5;
            m_NurbsParser.GetNurbsPoint(StartNode_.nKnot + 0.5 * _nDeltaKnot, m_nxTempPoints[3]);
            _nTheFleche = (m_nxTempPoints[2] - m_nxTempPoints[3]).GetSquareLength();
        }

        if (_nTheFleche < nDeflection2_) {
            m_pvecScatterNodes->push_back(_NextNode);
        }
        else {
            QuasiFleche(nDeflection2_, StartNode_, _NextNode, 3, nEps_);
        }

        if (nPointMin_ > 2) {
            int _nPointMin = nPointMin_ - (static_cast<int>(m_pvecScatterNodes->size()) - _nPointSize);
            QuasiFleche(nDeflection2_, _NextNode, EndNode_, _nPointMin, nEps_);
        }
    }

} // End namespace neat

#endif // __NURBS_DEFLECTION_H_20200130__