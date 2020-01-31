/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#include "pch.h"

using namespace neat;

//////////////////////////////////////////////////////////////////////////
// local class 
namespace neat
{
    /**
     * The implementation pointer of nurbs parser.
     */
    class CNurbsParserPointer final
    {
    public:
        CNurbsParserPointer(int nDimension_);
        ~CNurbsParserPointer();

        void SetNurbsNode(const NurbsNode1D* pNurbsNode_);
        void GetNurbsPoint(double nKnot_, double& nPoint_);
        void GetNurbsDeriv(double nKnot_, double& nPoint_, double& nDeriv1_);
        void GetNurbsDeriv(double nKnot_, double& nPoint_, double& nDeriv1_, double& nDeriv2_);

        void SetNurbsNode(const NurbsNode2D* pNurbsNode_);
        void GetNurbsPoint(double nKnot_, DPOINT2& ptPoint_);
        void GetNurbsDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_);
        void GetNurbsDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_, DPOINT2& ptDeriv2_);

        void SetNurbsNode(const NurbsNode3D* pNurbsNode_);
        void GetNurbsPoint(double nKnot_, DPOINT3& ptPoint_);
        void GetNurbsDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_);
        void GetNurbsDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_, DPOINT3& ptDeriv2_);

    private:
        CNurbsParserPointer(const CNurbsParserPointer&) = delete;
        CNurbsParserPointer& operator=(const CNurbsParserPointer&) = delete;

        CNurbsParser<double>* m_pParser1D;
        CNurbsParser<DPOINT2>* m_pParser2D;
        CNurbsParser<DPOINT3>* m_pParser3D;
    };

    /**
     * The implementation pointer of bezier parser.
     */
    class CBezierParserPointer final
    {
    public:
        CBezierParserPointer(int nDimension_);
        ~CBezierParserPointer();

        void SetBezierNode(const BezierNode2D* pBezierNode_);
        void GetBezierPoint(double nKnot_, DPOINT2& ptPoint_);
        void GetBezierDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_);
        void GetBezierDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_, DPOINT2& ptDeriv2_);

        void SetBezierNode(const BezierNode3D* pBezierNode_);
        void GetBezierPoint(double nKnot_, DPOINT3& ptPoint_);
        void GetBezierDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_);
        void GetBezierDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_, DPOINT3& ptDeriv2_);

    private:
        CBezierParserPointer(const CBezierParserPointer&) = delete;
        CBezierParserPointer& operator=(const CBezierParserPointer&) = delete;

        NurbsNode2D* m_pNurbsNode2D;
        NurbsNode3D* m_pNurbsNode3D;
        CNurbsParser<DPOINT2>* m_pParser2D;
        CNurbsParser<DPOINT3>* m_pParser3D;
    };

} // End namespace neat

//////////////////////////////////////////////////////////////////////////
// class CNurbsParser1D
CNurbsParser1D::CNurbsParser1D()
{
    m_pParserPointer = new CNurbsParserPointer(1);
}

CNurbsParser1D::~CNurbsParser1D()
{
    delete m_pParserPointer;
}

void CNurbsParser1D::SetNurbsNode(const NurbsNode1D* pNurbNode_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->SetNurbsNode(pNurbNode_);
}

void CNurbsParser1D::GetNurbsPoint(double nKnot_, double& nPoint_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetNurbsPoint(nKnot_, nPoint_);
}

void CNurbsParser1D::GetNurbsDeriv(double nKnot_, double& nPoint_, double& nDeriv1_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetNurbsDeriv(nKnot_, nPoint_, nDeriv1_);
}

void CNurbsParser1D::GetNurbsDeriv(double nKnot_, double& nPoint_, double& nDeriv1_, double& nDeriv2_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetNurbsDeriv(nKnot_, nPoint_, nDeriv1_, nDeriv2_);
}

//////////////////////////////////////////////////////////////////////////
// class CNurbsParser2D
CNurbsParser2D::CNurbsParser2D()
{
    m_pParserPointer = new CNurbsParserPointer(2);
}

CNurbsParser2D::~CNurbsParser2D()
{
    delete m_pParserPointer;
}

void CNurbsParser2D::SetNurbsNode(const NurbsNode2D* pNurbNode_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->SetNurbsNode(pNurbNode_);
}

void CNurbsParser2D::GetNurbsPoint(double nKnot_, DPOINT2& ptPoint_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetNurbsPoint(nKnot_, ptPoint_);
}

void CNurbsParser2D::GetNurbsDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_);
}

void CNurbsParser2D::GetNurbsDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_, DPOINT2& ptDeriv2_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_, ptDeriv2_);
}

//////////////////////////////////////////////////////////////////////////
// class CNurbsParser3D
CNurbsParser3D::CNurbsParser3D()
{
    m_pParserPointer = new CNurbsParserPointer(3);
}

CNurbsParser3D::~CNurbsParser3D()
{
    delete m_pParserPointer;
}

void CNurbsParser3D::SetNurbsNode(const NurbsNode3D* pNurbsNode_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->SetNurbsNode(pNurbsNode_);
}

void CNurbsParser3D::GetNurbsPoint(double nKnot_, DPOINT3& ptPoint_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetNurbsPoint(nKnot_, ptPoint_);
}

void CNurbsParser3D::GetNurbsDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_);
}

void CNurbsParser3D::GetNurbsDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_, DPOINT3& ptDeriv2_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_, ptDeriv2_);
}

//////////////////////////////////////////////////////////////////////////
// class CBezierParser2D
CBezierParser2D::CBezierParser2D()
{
    m_pParserPointer = new CBezierParserPointer(2);
}

CBezierParser2D::~CBezierParser2D()
{
    delete m_pParserPointer;
}

void CBezierParser2D::SetBezierNode(const BezierNode2D* pBezierNode_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->SetBezierNode(pBezierNode_);
}

void CBezierParser2D::GetBezierPoint(double nKnot_, DPOINT2& ptPoint_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetBezierPoint(nKnot_, ptPoint_);
}

void CBezierParser2D::GetBezierDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetBezierDeriv(nKnot_, ptPoint_, ptDeriv1_);
}

void CBezierParser2D::GetBezierDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_, DPOINT2& ptDeriv2_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetBezierDeriv(nKnot_, ptPoint_, ptDeriv1_, ptDeriv2_);
}

//////////////////////////////////////////////////////////////////////////
// class CBezierParser3D
CBezierParser3D::CBezierParser3D()
{
    m_pParserPointer = new CBezierParserPointer(3);
}

CBezierParser3D::~CBezierParser3D()
{
    delete m_pParserPointer;
}

void CBezierParser3D::SetBezierNode(const BezierNode3D* pBezierNode_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->SetBezierNode(pBezierNode_);
}

void CBezierParser3D::GetBezierPoint(double nKnot_, DPOINT3& ptPoint_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetBezierPoint(nKnot_, ptPoint_);
}

void CBezierParser3D::GetBezierDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetBezierDeriv(nKnot_, ptPoint_, ptDeriv1_);
}

void CBezierParser3D::GetBezierDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_, DPOINT3& ptDeriv2_)
{
    NEAT_RAISE_IF(m_pParserPointer == nullptr, g_strEmptyPointer);
    m_pParserPointer->GetBezierDeriv(nKnot_, ptPoint_, ptDeriv1_, ptDeriv2_);
}

//////////////////////////////////////////////////////////////////////////
// local class CNurbsParserPointer
CNurbsParserPointer::CNurbsParserPointer(int nDimension_)
{
    NEAT_RAISE(nDimension_ != 1 && nDimension_ != 2 && nDimension_ != 3, g_strWrongDimension);

    switch (nDimension_)
    {
    case 1:
        m_pParser1D = new CNurbsParser<double>();
        m_pParser2D = nullptr;
        m_pParser3D = nullptr;
        break;
    case 2:
        m_pParser2D = new CNurbsParser<DPOINT2>();
        m_pParser1D = nullptr;
        m_pParser3D = nullptr;
        break;
    case 3:
        m_pParser3D = new CNurbsParser<DPOINT3>();
        m_pParser1D = nullptr;
        m_pParser2D = nullptr;
        break;
    default:
        break;
    }
}

CNurbsParserPointer::~CNurbsParserPointer()
{
    if (m_pParser1D != nullptr) {
        delete m_pParser1D;
    }

    if (m_pParser2D != nullptr) {
        delete m_pParser2D;
    }

    if (m_pParser3D != nullptr) {
        delete m_pParser3D;
    }
}

void CNurbsParserPointer::SetNurbsNode(const NurbsNode1D* pNurbsNode_)
{
    NEAT_RAISE_IF(m_pParser1D == nullptr, g_strEmptyPointer);
    m_pParser1D->SetNurbsNode(pNurbsNode_);
}

void CNurbsParserPointer::GetNurbsPoint(double nKnot_, double& nPoint_)
{
    NEAT_RAISE_IF(m_pParser1D == nullptr, g_strEmptyPointer);
    m_pParser1D->GetNurbsPoint(nKnot_, nPoint_);
}

void CNurbsParserPointer::GetNurbsDeriv(double nKnot_, double& nPoint_, double& nDeriv1_)
{
    NEAT_RAISE_IF(m_pParser1D == nullptr, g_strEmptyPointer);
    m_pParser1D->GetNurbsDeriv(nKnot_, nPoint_, nDeriv1_);
}

void CNurbsParserPointer::GetNurbsDeriv(double nKnot_, double& nPoint_, double& nDeriv1_, double& nDeriv2_)
{
    NEAT_RAISE_IF(m_pParser1D == nullptr, g_strEmptyPointer);
    m_pParser1D->GetNurbsDeriv(nKnot_, nPoint_, nDeriv1_, nDeriv2_);
}

void CNurbsParserPointer::SetNurbsNode(const NurbsNode2D* pNurbsNode_)
{
    NEAT_RAISE_IF(m_pParser2D == nullptr, g_strEmptyPointer);
    m_pParser2D->SetNurbsNode(pNurbsNode_);
}

void CNurbsParserPointer::GetNurbsPoint(double nKnot_, DPOINT2& ptPoint_)
{
    NEAT_RAISE_IF(m_pParser2D == nullptr, g_strEmptyPointer);
    m_pParser2D->GetNurbsPoint(nKnot_, ptPoint_);
}

void CNurbsParserPointer::GetNurbsDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_)
{
    NEAT_RAISE_IF(m_pParser2D == nullptr, g_strEmptyPointer);
    m_pParser2D->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_);
}

void CNurbsParserPointer::GetNurbsDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_, DPOINT2& ptDeriv2_)
{
    NEAT_RAISE_IF(m_pParser2D == nullptr, g_strEmptyPointer);
    m_pParser2D->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_, ptDeriv2_);
}

void CNurbsParserPointer::SetNurbsNode(const NurbsNode3D* pNurbsNode_)
{
    NEAT_RAISE_IF(m_pParser3D == nullptr, g_strEmptyPointer);
    m_pParser3D->SetNurbsNode(pNurbsNode_);
}

void CNurbsParserPointer::GetNurbsPoint(double nKnot_, DPOINT3& ptPoint_)
{
    NEAT_RAISE_IF(m_pParser3D == nullptr, g_strEmptyPointer);
    m_pParser3D->GetNurbsPoint(nKnot_, ptPoint_);
}

void CNurbsParserPointer::GetNurbsDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_)
{
    NEAT_RAISE_IF(m_pParser3D == nullptr, g_strEmptyPointer);
    m_pParser3D->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_);
}

void CNurbsParserPointer::GetNurbsDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_, DPOINT3& ptDeriv2_)
{
    NEAT_RAISE_IF(m_pParser3D == nullptr, g_strEmptyPointer);
    m_pParser3D->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_, ptDeriv2_);
}

//////////////////////////////////////////////////////////////////////////
// local class CBezierParserPointer
CBezierParserPointer::CBezierParserPointer(int nDimension_)
{
    NEAT_RAISE(nDimension_ != 2 && nDimension_ != 3, g_strWrongDimension);

    if (nDimension_ == 2) {
        m_pNurbsNode2D = new NurbsNode2D;
        m_pParser2D = new CNurbsParser<DPOINT2>();
        m_pNurbsNode3D = nullptr;
        m_pParser3D = nullptr;
    }
    else if (nDimension_ == 3) {
        m_pNurbsNode3D = new NurbsNode3D;
        m_pParser3D = new CNurbsParser<DPOINT3>();
        m_pNurbsNode2D = nullptr;
        m_pParser2D = nullptr;
    }
}

CBezierParserPointer::~CBezierParserPointer()
{
    if (m_pNurbsNode2D != nullptr) {
        delete m_pNurbsNode2D;
    }

    if (m_pNurbsNode3D != nullptr) {
        delete m_pNurbsNode3D;
    }

    if (m_pParser2D != nullptr) {
        delete m_pParser2D;
    }

    if (m_pParser3D != nullptr) {
        delete m_pParser3D;
    }
}

void CBezierParserPointer::SetBezierNode(const BezierNode2D* pBezierNode_)
{
    NEAT_RAISE_IF(m_pNurbsNode2D == nullptr || m_pParser2D == nullptr, g_strEmptyPointer);
    ConvertBezierToNurbs(pBezierNode_, m_pNurbsNode2D);
    m_pParser2D->SetNurbsNode(m_pNurbsNode2D);
}

void CBezierParserPointer::GetBezierPoint(double nKnot_, DPOINT2& ptPoint_)
{
    NEAT_RAISE_IF(m_pParser2D == nullptr, g_strEmptyPointer);
    m_pParser2D->GetNurbsPoint(nKnot_, ptPoint_);
}

void CBezierParserPointer::GetBezierDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_)
{
    NEAT_RAISE_IF(m_pParser2D == nullptr, g_strEmptyPointer);
    m_pParser2D->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_);
}

void CBezierParserPointer::GetBezierDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_, DPOINT2& ptDeriv2_)
{
    NEAT_RAISE_IF(m_pParser2D == nullptr, g_strEmptyPointer);
    m_pParser2D->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_, ptDeriv2_);
}

void CBezierParserPointer::SetBezierNode(const BezierNode3D* pBezierNode_)
{
    NEAT_RAISE_IF(m_pNurbsNode3D == nullptr || m_pParser3D == nullptr, g_strEmptyPointer);
    ConvertBezierToNurbs(pBezierNode_, m_pNurbsNode3D);
    m_pParser3D->SetNurbsNode(m_pNurbsNode3D);
}

void CBezierParserPointer::GetBezierPoint(double nKnot_, DPOINT3& ptPoint_)
{
    NEAT_RAISE_IF(m_pParser3D == nullptr, g_strEmptyPointer);
    m_pParser3D->GetNurbsPoint(nKnot_, ptPoint_);
}

void CBezierParserPointer::GetBezierDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_)
{
    NEAT_RAISE_IF(m_pParser3D == nullptr, g_strEmptyPointer);
    m_pParser3D->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_);
}

void CBezierParserPointer::GetBezierDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_, DPOINT3& ptDeriv2_)
{
    NEAT_RAISE_IF(m_pParser3D == nullptr, g_strEmptyPointer);
    m_pParser3D->GetNurbsDeriv(nKnot_, ptPoint_, ptDeriv1_, ptDeriv2_);
}