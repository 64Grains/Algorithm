/***************************************************************************************************
 Created on: 2020-01-30
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatSpline Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_CONVERTER_H_20200130__
#define __NEAT_CONVERTER_H_20200130__

/**
 * This file defines the class interface for converting splines.
 */
namespace neat
{
    /**
     * The converter for two-dimensional nurbs node.
     */

    // Convert nurbs node to bezier nodes.
    NEATSPLINE_API void ConvertNurbsToBezier(const NurbsNode2D* pNurbsNode_, std::vector<BezierNode2D>& vecBezierNodes_);

    /**
     * The converter for three-dimensional nurbs node.
     */

    // Convert nurbs node to bezier nodes.
    NEATSPLINE_API void ConvertNurbsToBezier(const NurbsNode3D* pNurbsNode_, std::vector<BezierNode3D>& vecBezierNodes_);

} // End namespace neat

#endif // __NEAT_CONVERTER_H_20200130__