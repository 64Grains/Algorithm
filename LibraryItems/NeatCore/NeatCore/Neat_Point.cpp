/***************************************************************************************************
 Created on: 2020-01-27
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatCore Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#include "pch.h"

using namespace neat;

//////////////////////////////////////////////////////////////////////////
// class CAxisPoint
template<>
double CAxisPoint<double, 2>::GetLength() const
{
    // Hypot can also be used here, this function does not have excessive overflow or underflow in the middle stage of the calculation.
    return sqrt(m_ptPoint[axis::x] * m_ptPoint[axis::x] + m_ptPoint[axis::y] * m_ptPoint[axis::y]);
}

template<>
double CAxisPoint<double, 2>::GetSquareLength() const
{
    return m_ptPoint[axis::x] * m_ptPoint[axis::x] + m_ptPoint[axis::y] * m_ptPoint[axis::y];
}

template<>
double CAxisPoint<double, 3>::GetLength() const
{
    return sqrt(m_ptPoint[axis::x] * m_ptPoint[axis::x]
        + m_ptPoint[axis::y] * m_ptPoint[axis::y]
        + m_ptPoint[axis::z] * m_ptPoint[axis::z]);
}

template<>
double CAxisPoint<double, 3>::GetSquareLength() const
{
    return m_ptPoint[axis::x] * m_ptPoint[axis::x]
        + m_ptPoint[axis::y] * m_ptPoint[axis::y]
        + m_ptPoint[axis::z] * m_ptPoint[axis::z];
}