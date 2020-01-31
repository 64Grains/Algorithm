/***************************************************************************************************
 Created on: 2020-01-31
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
     * The implementation pointer of nurbs interp.
     */
    class CNurbsInterpPointer final
    {
    public:
        CNurbsInterpPointer(int nDimension_);
        ~CNurbsInterpPointer();

        void SetKnotVector(const VECDOUBLE& vecKnots_);

        void SetStartTangent(double nStartTangent_);
        void SetEndTangent(double nEndTangent_);
        bool InterpNurbsCurve(const VECDOUBLE& vecFitPoints_, NurbsNode1D& NurbsNode_);

        void SetStartTangent(const DPOINT2& nxStartTangent_);
        void SetEndTangent(const DPOINT2& nxEndTangent_);
        bool InterpNurbsCurve(const VECDPOINT2& vecFitPoints_, NurbsNode2D& NurbsNode_);

        void SetStartTangent(const DPOINT3& nxStartTangent_);
        void SetEndTangent(const DPOINT3& nxEndTangent_);
        bool InterpNurbsCurve(const VECDPOINT3& vecFitPoints_, NurbsNode3D& NurbsNode_);

    private:
        CNurbsInterpPointer(const CNurbsInterpPointer&) = delete;
        CNurbsInterpPointer& operator=(const CNurbsInterpPointer&) = delete;

        CNurbsInterp<double>* m_pInterp1D;
        CNurbsInterp<DPOINT2>* m_pInterp2D;
        CNurbsInterp<DPOINT3>* m_pInterp3D;
        int m_nDimension;
    };

} // End namespace neat

//////////////////////////////////////////////////////////////////////////
// class CNurbsInterp1D
CNurbsInterp1D::CNurbsInterp1D()
{
    m_pInterpPointer = new CNurbsInterpPointer(1);
}

CNurbsInterp1D::~CNurbsInterp1D()
{
    delete m_pInterpPointer;
}

void CNurbsInterp1D::SetStartTangent(double nStartTangent_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    m_pInterpPointer->SetStartTangent(nStartTangent_);
}

void CNurbsInterp1D::SetEndTangent(double nEndTangent_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    m_pInterpPointer->SetEndTangent(nEndTangent_);
}

void CNurbsInterp1D::SetKnotVector(const VECDOUBLE& vecKnots_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    m_pInterpPointer->SetKnotVector(vecKnots_);
}

bool CNurbsInterp1D::InterpNurbsCurve(const VECDOUBLE& vecFitPoints_, NurbsNode1D& NurbsNode_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    return m_pInterpPointer->InterpNurbsCurve(vecFitPoints_, NurbsNode_);
}

//////////////////////////////////////////////////////////////////////////
// class CNurbsInterp2D
CNurbsInterp2D::CNurbsInterp2D()
{
    m_pInterpPointer = new CNurbsInterpPointer(2);
}

CNurbsInterp2D::~CNurbsInterp2D()
{
    delete m_pInterpPointer;
}

void CNurbsInterp2D::SetStartTangent(const DPOINT2& nxStartTangent_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    m_pInterpPointer->SetStartTangent(nxStartTangent_);
}

void CNurbsInterp2D::SetEndTangent(const DPOINT2& nxEndTangent_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    m_pInterpPointer->SetEndTangent(nxEndTangent_);
}

void CNurbsInterp2D::SetKnotVector(const VECDOUBLE& vecKnots_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    m_pInterpPointer->SetKnotVector(vecKnots_);
}

bool CNurbsInterp2D::InterpNurbsCurve(const VECDPOINT2& vecFitPoints_, NurbsNode2D& NurbsNode_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    return m_pInterpPointer->InterpNurbsCurve(vecFitPoints_, NurbsNode_);
}

//////////////////////////////////////////////////////////////////////////
// class CNurbsInterp3D
CNurbsInterp3D::CNurbsInterp3D()
{
    m_pInterpPointer = new CNurbsInterpPointer(3);
}

CNurbsInterp3D::~CNurbsInterp3D()
{
    delete m_pInterpPointer;
}

void CNurbsInterp3D::SetStartTangent(const DPOINT3& nxStartTangent_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    m_pInterpPointer->SetStartTangent(nxStartTangent_);
}

void CNurbsInterp3D::SetEndTangent(const DPOINT3& nxEndTangent_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    m_pInterpPointer->SetEndTangent(nxEndTangent_);
}

void CNurbsInterp3D::SetKnotVector(const VECDOUBLE& vecKnots_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    m_pInterpPointer->SetKnotVector(vecKnots_);
}

bool CNurbsInterp3D::InterpNurbsCurve(const VECDPOINT3& vecFitPoints_, NurbsNode3D& NurbsNode_)
{
    NEAT_RAISE_IF(m_pInterpPointer == nullptr, g_strEmptyPointer);
    return m_pInterpPointer->InterpNurbsCurve(vecFitPoints_, NurbsNode_);
}

//////////////////////////////////////////////////////////////////////////
// local class CNurbsInterpPointer
CNurbsInterpPointer::CNurbsInterpPointer(int nDimension_)
{
    NEAT_RAISE(nDimension_ != 1 && nDimension_ != 2 && nDimension_ != 3, g_strWrongDimension);
    m_nDimension = nDimension_;

    switch (m_nDimension)
    {
    case 1:
        m_pInterp1D = new CNurbsInterp<double>();
        m_pInterp2D = nullptr;
        m_pInterp3D = nullptr;
        break;
    case 2:
        m_pInterp2D = new CNurbsInterp<DPOINT2>();
        m_pInterp1D = nullptr;
        m_pInterp3D = nullptr;
        break;
    case 3:
        m_pInterp3D = new CNurbsInterp<DPOINT3>();
        m_pInterp1D = nullptr;
        m_pInterp2D = nullptr;
        break;
    default:
        break;
    }
}

CNurbsInterpPointer::~CNurbsInterpPointer()
{
    if (m_pInterp1D != nullptr) {
        delete m_pInterp1D;
    }

    if (m_pInterp2D != nullptr) {
        delete m_pInterp2D;
    }

    if (m_pInterp3D != nullptr) {
        delete m_pInterp3D;
    }
}

void CNurbsInterpPointer::SetKnotVector(const VECDOUBLE& vecKnots_)
{
    switch (m_nDimension)
    {
    case 1:
        NEAT_RAISE_IF(m_pInterp1D == nullptr, g_strEmptyPointer);
        m_pInterp1D->SetKnotVector(vecKnots_);
        break;
    case 2:
        NEAT_RAISE_IF(m_pInterp2D == nullptr, g_strEmptyPointer);
        m_pInterp2D->SetKnotVector(vecKnots_);
        break;
    case 3:
        NEAT_RAISE_IF(m_pInterp3D == nullptr, g_strEmptyPointer);
        m_pInterp3D->SetKnotVector(vecKnots_);
        break;
    default:
        break;
    }
}

void CNurbsInterpPointer::SetStartTangent(double nStartTangent_)
{
    NEAT_RAISE_IF(m_nDimension != 1, g_strWrongDimension);
    NEAT_RAISE_IF(m_pInterp1D == nullptr, g_strEmptyPointer);
    m_pInterp1D->SetStartTangent(nStartTangent_);
}

void CNurbsInterpPointer::SetEndTangent(double nEndTangent_)
{
    NEAT_RAISE_IF(m_nDimension != 1, g_strWrongDimension);
    NEAT_RAISE_IF(m_pInterp1D == nullptr, g_strEmptyPointer);
    m_pInterp1D->SetEndTangent(nEndTangent_);
}

bool CNurbsInterpPointer::InterpNurbsCurve(const VECDOUBLE& vecFitPoints_, NurbsNode1D& NurbsNode_)
{
    NEAT_RAISE_IF(m_nDimension != 1, g_strWrongDimension);
    NEAT_RAISE_IF(m_pInterp1D == nullptr, g_strEmptyPointer);
    return m_pInterp1D->InterpNurbsCurve(vecFitPoints_, NurbsNode_);
}

void CNurbsInterpPointer::SetStartTangent(const DPOINT2& nxStartTangent_)
{
    NEAT_RAISE_IF(m_nDimension != 2, g_strWrongDimension);
    NEAT_RAISE_IF(m_pInterp2D == nullptr, g_strEmptyPointer);
    m_pInterp2D->SetStartTangent(nxStartTangent_);
}

void CNurbsInterpPointer::SetEndTangent(const DPOINT2& nxEndTangent_)
{
    NEAT_RAISE_IF(m_nDimension != 2, g_strWrongDimension);
    NEAT_RAISE_IF(m_pInterp2D == nullptr, g_strEmptyPointer);
    m_pInterp2D->SetEndTangent(nxEndTangent_);
}

bool CNurbsInterpPointer::InterpNurbsCurve(const VECDPOINT2& vecFitPoints_, NurbsNode2D& NurbsNode_)
{
    NEAT_RAISE_IF(m_nDimension != 2, g_strWrongDimension);
    NEAT_RAISE_IF(m_pInterp2D == nullptr, g_strEmptyPointer);
    return m_pInterp2D->InterpNurbsCurve(vecFitPoints_, NurbsNode_);
}

void CNurbsInterpPointer::SetStartTangent(const DPOINT3& nxStartTangent_)
{
    NEAT_RAISE_IF(m_nDimension != 3, g_strWrongDimension);
    NEAT_RAISE_IF(m_pInterp3D == nullptr, g_strEmptyPointer);
    m_pInterp3D->SetStartTangent(nxStartTangent_);
}

void CNurbsInterpPointer::SetEndTangent(const DPOINT3& nxEndTangent_)
{
    NEAT_RAISE_IF(m_nDimension != 3, g_strWrongDimension);
    NEAT_RAISE_IF(m_pInterp3D == nullptr, g_strEmptyPointer);
    m_pInterp3D->SetEndTangent(nxEndTangent_);
}

bool CNurbsInterpPointer::InterpNurbsCurve(const VECDPOINT3& vecFitPoints_, NurbsNode3D& NurbsNode_)
{
    NEAT_RAISE_IF(m_nDimension != 3, g_strWrongDimension);
    NEAT_RAISE_IF(m_pInterp3D == nullptr, g_strEmptyPointer);
    return m_pInterp3D->InterpNurbsCurve(vecFitPoints_, NurbsNode_);
}