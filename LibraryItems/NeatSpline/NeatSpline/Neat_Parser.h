/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_PARSER_H_20200130__
#define __NEAT_PARSER_H_20200130__

/**
 * This file defines the interface for parsing splines.
 */
namespace neat
{
    class CNurbsParserPointer;
    class CBezierParserPointer;

    /**
     * The parser for one-dimensional nurbs node
     */
    class NEATSPLINE_API CNurbsParser1D final
    {
    public:
        CNurbsParser1D();
        ~CNurbsParser1D();

        // Set the information for one-dimensional nurbs node.
        void SetNurbsNode(const NurbsNode1D* pNurbsNode_);

        // Returns in nPoint_ the point of parameter nKnot_.
        void GetNurbsPoint(double nKnot_, double& nPoint_);

        // Returns in nPoint_ the point and nDeriv1_ the first-order derivative of parameter nKnot_.
        void GetNurbsDeriv(double nKnot_, double& nPoint_, double& nDeriv1_);

        // Returns in nPoint_ the point, nDeriv1_ the first-order derivative and nDeriv2_ the second-order derivative of parameter nKnot_.
        void GetNurbsDeriv(double nKnot_, double& nPoint_, double& nDeriv1_, double& nDeriv2_);

    private:
        CNurbsParser1D(const CNurbsParser1D&) = delete;
        CNurbsParser1D& operator=(const CNurbsParser1D&) = delete;

        CNurbsParserPointer* m_pParserPointer;
    };

    /**
     * The parser for two-dimensional nurbs node
     */
    class NEATSPLINE_API CNurbsParser2D final
    {
    public:
        CNurbsParser2D();
        ~CNurbsParser2D();

        // Set the information for two-dimensional nurbs node.
        void SetNurbsNode(const NurbsNode2D* pNurbsNode_);

        // Returns in ptPoint_ the point of parameter nKnot_.
        void GetNurbsPoint(double nKnot_, DPOINT2& ptPoint_);

        // Returns in ptPoint_ the point and ptDeriv1_ the first-order derivative of parameter nKnot_.
        void GetNurbsDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_);

        // Returns in ptPoint_ the point, ptDeriv1_ the first-order derivative and ptDeriv2_ the second-order derivative of parameter nKnot_.
        void GetNurbsDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_, DPOINT2& ptDeriv2_);

    private:
        CNurbsParser2D(const CNurbsParser2D&) = delete;
        CNurbsParser2D& operator=(const CNurbsParser2D&) = delete;

        CNurbsParserPointer* m_pParserPointer;
    };

    /**
     * The parser for three-dimensional nurbs node
     */
    class NEATSPLINE_API CNurbsParser3D final
    {
    public:
        CNurbsParser3D();
        ~CNurbsParser3D();

        // Set the information for two-dimensional nurbs node.
        void SetNurbsNode(const NurbsNode3D* pNurbsNode_);

        // Returns in ptPoint_ the point of parameter nKnot_.
        void GetNurbsPoint(double nKnot_, DPOINT3& ptPoint_);

        // Returns in ptPoint_ the point and ptDeriv1_ the first-order derivative of parameter nKnot_.
        void GetNurbsDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_);

        // Returns in ptPoint_ the point, ptDeriv1_ the first-order derivative and ptDeriv2_ the second-order derivative of parameter nKnot_.
        void GetNurbsDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_, DPOINT3& ptDeriv2_);

    private:
        CNurbsParser3D(const CNurbsParser3D&) = delete;
        CNurbsParser3D& operator=(const CNurbsParser3D&) = delete;

        CNurbsParserPointer* m_pParserPointer;
    };

    /**
     * The parser for two-dimensional bezier node
     */
    class NEATSPLINE_API CBezierParser2D final
    {
    public:
        CBezierParser2D();
        ~CBezierParser2D();

        // Set the information for two-dimensional bezier node.
        void SetBezierNode(const BezierNode2D* pBezierNode_);

        // Returns in ptPoint_ the point of parameter nKnot_.
        void GetBezierPoint(double nKnot_, DPOINT2& ptPoint_);

        // Returns in ptPoint_ the point and ptDeriv1_ the first-order derivative of parameter nKnot_.
        void GetBezierDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_);

        // Returns in ptPoint_ the point, ptDeriv1_ the first-order derivative and ptDeriv2_ the second-order derivative of parameter nKnot_.
        void GetBezierDeriv(double nKnot_, DPOINT2& ptPoint_, DPOINT2& ptDeriv1_, DPOINT2& ptDeriv2_);

    private:
        CBezierParser2D(const CBezierParser2D&) = delete;
        CBezierParser2D& operator=(const CBezierParser2D&) = delete;

        CBezierParserPointer* m_pParserPointer;
    };

    /**
     * The parser for three-dimensional bezier node
     */
    class NEATSPLINE_API CBezierParser3D final
    {
    public:
        CBezierParser3D();
        ~CBezierParser3D();

        // Set the information for two-dimensional bezier node.
        void SetBezierNode(const BezierNode3D* pBezierNode_);

        // Returns in ptPoint_ the point of parameter nKnot_.
        void GetBezierPoint(double nKnot_, DPOINT3& ptPoint_);

        // Returns in ptPoint_ the point and ptDeriv1_ the first-order derivative of parameter nKnot_.
        void GetBezierDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_);

        // Returns in ptPoint_ the point, ptDeriv1_ the first-order derivative and ptDeriv2_ the second-order derivative of parameter nKnot_.
        void GetBezierDeriv(double nKnot_, DPOINT3& ptPoint_, DPOINT3& ptDeriv1_, DPOINT3& ptDeriv2_);

    private:
        CBezierParser3D(const CBezierParser3D&) = delete;
        CBezierParser3D& operator=(const CBezierParser3D&) = delete;

        CBezierParserPointer* m_pParserPointer;
    };

} // End namespace neat

#endif // __NEAT_PARSER_H_20200130__