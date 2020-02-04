/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_SCATTER_H_20200130__
#define __NEAT_SCATTER_H_20200130__

/**
 * This file defines the interface for scatting splines.
 */
namespace neat
{
    /**
     * The scatter for two-dimensional nurbs node.
     */

    // Scatter nurbs node by given deflection, and output the scatter points.
    NEATSPLINE_API void ScatterNurbs(const NurbsNode2D* pNurbsNode_, double nDeflection_, VECDPOINT2& vecScatterPoints_);

    // Scatter nurbs node by given deflection, and output the scatter nodes.
    NEATSPLINE_API void ScatterNurbs(const NurbsNode2D* pNurbsNode_, double nDeflection_, std::vector<ScatterNode2D>& vecScatterNodes_);

    // Scatter nurbs node by given deflection, and output the polyline.
    NEATSPLINE_API void ScatterNurbs(const NurbsNode2D* pNurbsNode_, double nDeflection_, Polyline2D& Polyline2D_);

    /**
     * The scatter for three-dimensional nurbs node.
     */

    // Scatter nurbs node by given deflection, and output the scatter points.
    NEATSPLINE_API void ScatterNurbs(const NurbsNode3D* pNurbsNode_, double nDeflection_, VECDPOINT3& vecScatterPoints_);

    // Scatter nurbs node by given deflection, and output the scatter nodes.
    NEATSPLINE_API void ScatterNurbs(const NurbsNode3D* pNurbsNode_, double nDeflection_, std::vector<ScatterNode3D>& vecScatterNodes_);

    /**
     * The scatter for two-dimensional bezier node.
     */

    // Scatter bezier node by given deflection, and output the scatter points.
    NEATSPLINE_API void ScatterBezier(const BezierNode2D* pBezierNode_, double nDeflection_, VECDPOINT2& vecScatterPoints_);

    // Scatter bezier node by given deflection, and output the scatter nodes.
    NEATSPLINE_API void ScatterBezier(const BezierNode2D* pBezierNode_, double nDeflection_, std::vector<ScatterNode2D>& vecScatterNodes_);

    // Scatter bezier node by given deflection, and output the polyline.
    NEATSPLINE_API void ScatterBezier(const BezierNode2D* pBezierNode_, double nDeflection_, Polyline2D& Polyline2D_);

    /**
     * The scatter for three-dimensional bezier node.
     */

    // Scatter bezier node by given deflection, and output the scatter points.
    NEATSPLINE_API void ScatterBezier(const BezierNode3D* pBezierNode_, double nDeflection_, VECDPOINT3& vecScatterPoints_);

    // Scatter bezier node by given deflection, and output the scatter nodes.
    NEATSPLINE_API void ScatterBezier(const BezierNode3D* pBezierNode_, double nDeflection_, std::vector<ScatterNode3D>& vecScatterNodes_);

} // End namespace neat

#endif // __NEAT_SCATTER_H_20200130__