/***************************************************************************************************
 Created on: 2020-02-02
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatCore Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_NOTE2D_H_20200202__
#define __NEAT_NOTE2D_H_20200202__

/**
 * This file defines various nodes in 2D Cartesian coordinates.
 */
namespace neat
{
    /**
     * Arc node in 2D Cartesian coordinates
     */
    struct ArcNode2D
    {
        DPOINT2 ptCenter;           // The center of arc
        double nRadius = 0.0;       // The radius of arc
        double nStartAngle = 0.0;   // The start angle of arc
        double nEndAngle = 0.0;     // The end angle of arc
    };

    /**
     * Polyline in 2D Cartesian coordinates
     */
    struct Polyline2D
    {
        /**
         * Line or arc node in polyline
         */
        struct Node2D
        {
            double nEndX;   // X coordinate of the end point
            double nEndY;   // Y coordinate of the end point
            double nBulge;  // Bulge, zero value represents line node
        };
        typedef std::vector<Node2D> NodeVector2D;

        double nStartX;         // X coordinate of the start point
        double nStartY;         // Y coordinate of the start point
        NodeVector2D vecNodes;  // Node sequence of polyline
    };

} // End namespace neat

#endif // __NEAT_NOTE2D_H_20200202__