/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NURBS_PARSER_H_20200130__
#define __NURBS_PARSER_H_20200130__

/**
 * This file defines the parser for nurbs node.
 */
namespace neat
{
    template<typename TYPE>
    class CNurbsParser
    {
    public:
        CNurbsParser();
        ~CNurbsParser();

        // Set the information for two-dimensional nurbs node.
        void SetNurbsNode(const NurbsNode<TYPE>* pNurbsNode_);

        // Returns in ptPoint_ the point of parameter nKnot_.
        void GetNurbsPoint(double nKnot_, TYPE& ptPoint_);

        // Returns in ptPoint_ the point and ptDeriv1_ the first-order derivative of parameter nKnot_.
        void GetNurbsDeriv(double nKnot_, TYPE& ptPoint_, TYPE& ptDeriv1_);

        // Returns in ptPoint_ the point, ptDeriv1_ the first-order derivative and ptDeriv2_ the second-order derivative of parameter nKnot_.
        void GetNurbsDeriv(double nKnot_, TYPE& ptPoint_, TYPE& ptDeriv1_, TYPE& ptDeriv2_);

    private:
        NurbsNode<TYPE>* m_pNurbsNode;
        CNRBSplineParser<TYPE>* m_pNRBSplineParser;
        CRBSplineParser<TYPE>* m_pRBSplineParser;
        CRBSplineParser2<TYPE>* m_pRBSplineParser2;
        bool m_bSetNurbsNode[3];
        bool m_bRational;
    };

    /**
     * The implementation of template class.
     */
    template<typename TYPE>
    CNurbsParser<TYPE>::CNurbsParser()
        : m_pNurbsNode(nullptr)
        , m_pNRBSplineParser(nullptr)
        , m_pRBSplineParser(nullptr)
        , m_pRBSplineParser2(nullptr)
        , m_bRational(false)
    {
        for (int i = 0; i < 3; ++i) {
            m_bSetNurbsNode[i] = false;
        }
    }

    template<typename TYPE>
    CNurbsParser<TYPE>::~CNurbsParser()
    {
        if (m_pNRBSplineParser != nullptr) {
            delete m_pNRBSplineParser;
        }

        if (m_pRBSplineParser != nullptr) {
            delete m_pRBSplineParser;
        }

        if (m_pRBSplineParser2 != nullptr) {
            delete m_pRBSplineParser2;
        }
    }

    template<typename TYPE>
    void CNurbsParser<TYPE>::SetNurbsNode(const NurbsNode<TYPE>* pNurbsNode_)
    {
        m_pNurbsNode = (NurbsNode<TYPE>*)pNurbsNode_;
        m_bRational = CheckNurbsNodeRational(pNurbsNode_);
        for (int i = 0; i < 3; ++i) {
            m_bSetNurbsNode[i] = false;
        }
    }

    template<typename TYPE>
    void CNurbsParser<TYPE>::GetNurbsPoint(double nKnot_, TYPE& ptPoint_)
    {
        if (!m_bRational) {
            // Non Rational BSpline curve
            if (m_pNRBSplineParser == nullptr) {
                m_pNRBSplineParser = new CNRBSplineParser<TYPE>();
            }

            if (!m_bSetNurbsNode[0]) {
                m_pNRBSplineParser->SetNurbsNode(m_pNurbsNode, m_bRational);
                m_bSetNurbsNode[0] = true;
            }

            m_pNRBSplineParser->GetNurbsPoint(nKnot_, ptPoint_);
        }
        else {
            // Rational BSpline curve: less less than first-order derivative.
            if (m_pRBSplineParser == nullptr) {
                m_pRBSplineParser = new CRBSplineParser<TYPE>();
            }

            if (!m_bSetNurbsNode[1]) {
                m_pRBSplineParser->SetNurbsNode(m_pNurbsNode, m_bRational);
                m_bSetNurbsNode[1] = true;
            }

            m_pRBSplineParser->GetNurbsPoint(nKnot_, ptPoint_);
        }
    }

    template<typename TYPE>
    void CNurbsParser<TYPE>::GetNurbsDeriv(double nKnot_, TYPE& ptPoint_, TYPE& ptDeriv1_)
    {
        if (!m_bRational) {
            // Non Rational BSpline curve
            if (m_pNRBSplineParser == nullptr) {
                m_pNRBSplineParser = new CNRBSplineParser<TYPE>();
            }

            if (!m_bSetNurbsNode[0]) {
                m_pNRBSplineParser->SetNurbsNode(m_pNurbsNode, m_bRational);
                m_bSetNurbsNode[0] = true;
            }

            m_pNRBSplineParser->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_);
        }
        else {
            // Rational BSpline curve: less less than first-order derivative.
            if (m_pRBSplineParser == nullptr) {
                m_pRBSplineParser = new CRBSplineParser<TYPE>();
            }

            if (!m_bSetNurbsNode[1]) {
                m_pRBSplineParser->SetNurbsNode(m_pNurbsNode, m_bRational);
                m_bSetNurbsNode[1] = true;
            }

            m_pRBSplineParser->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_);
        }
    }

    template<typename TYPE>
    void CNurbsParser<TYPE>::GetNurbsDeriv(double nKnot_, TYPE& ptPoint_, TYPE& ptDeriv1_, TYPE& ptDeriv2_)
    {
        if (!m_bRational) {
            // Non Rational BSpline curve
            if (m_pNRBSplineParser == nullptr) {
                m_pNRBSplineParser = new CNRBSplineParser<TYPE>();
            }

            if (!m_bSetNurbsNode[0]) {
                m_pNRBSplineParser->SetNurbsNode(m_pNurbsNode, m_bRational);
                m_bSetNurbsNode[0] = true;
            }

            NEAT_RAISE(m_pNurbsNode->nDegree < 2, g_strWrongNurbsDegree);
            m_pNRBSplineParser->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_, ptDeriv2_);
        }
        else {
            // Rational BSpline curve: more than second-order derivative.
            if (m_pRBSplineParser2 == nullptr) {
                m_pRBSplineParser2 = new CRBSplineParser2<TYPE>();
            }

            if (!m_bSetNurbsNode[2]) {
                m_pRBSplineParser2->SetNurbsNode(m_pNurbsNode, m_bRational);
                m_bSetNurbsNode[2] = true;
            }

            NEAT_RAISE(m_pNurbsNode->nDegree < 2, g_strWrongNurbsDegree);
            m_pRBSplineParser2->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_, ptDeriv2_);
        }
    }

} // End namespace neat

#endif // __NURBS_PARSER_H_20200130__