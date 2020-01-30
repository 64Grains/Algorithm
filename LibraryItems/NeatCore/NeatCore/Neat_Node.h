/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatCore Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_NODE_H_20200130__
#define __NEAT_NODE_H_20200130__

/**
 * This file defines various spline nodes.
 */
namespace neat
{
    /**
     * Point or vector types in 2D and 3D Cartesian coordinates
     */
    typedef CAxisPoint<double, 2> DPOINT2;
    typedef CAxisPoint<double, 3> DPOINT3;

    typedef std::vector<double> VECDOUBLE;
    typedef std::vector<DPOINT2> VECDPOINT2;
    typedef std::vector<DPOINT3> VECDPOINT3;

    /**
     * Nurbs node
     */
    template<typename TYPE>
    struct NurbsNode
    {
        std::vector<TYPE> vecPoles; // The poles of nurbs node
        VECDOUBLE vecWeights;       // The weights of nurbs node
        VECDOUBLE vecKnots;         // The knot vector of nurbs node
        int nDegree = 0;            // The degree of nurbs node
    };
    typedef NurbsNode<double> NurbsNode1D;
    typedef NurbsNode<DPOINT2> NurbsNode2D;
    typedef NurbsNode<DPOINT3> NurbsNode3D;

    /**
     * Bezier node
     */
    template<typename TYPE>
    struct BezierNode
    {
        std::vector<TYPE> vecPoles; // The poles of bezier node
        VECDOUBLE vecWeights;       // The weights of bezier node
        double nStartKnot = 0.0;    // The start knot of bezier node
        double nEndKnot = 0.0;      // The end knot of bezier node
    };
    typedef BezierNode<DPOINT2> BezierNode2D;
    typedef BezierNode<DPOINT3> BezierNode3D;

    /**
     * Scatter node
     */
    template<typename TYPE>
    struct ScatterNode
    {
        TYPE ptPoint;       // The point corresponding to the knot
        TYPE ptDeriv1;      // The first-order derivative corresponding to the knot
        TYPE ptDeriv2;      // The second-order derivative corresponding to the knot
        double nKnot = 0.0; // The knot value
    };
    typedef ScatterNode<DPOINT2> ScatterNode2D;
    typedef ScatterNode<DPOINT3> ScatterNode3D;

} // End namespace neat

#endif // __NEAT_NODE_H_20200130__