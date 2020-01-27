/***************************************************************************************************
 Created on: 2020-01-27
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatCore Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_POINT_H_20200127__
#define __NEAT_POINT_H_20200127__

/**
 * This file defines the coordinate points in the Cartesian coordinate system.
 */
namespace neat
{
    /**
     * Axis
     */
    struct axis
    {
        enum { x = 0, y, z };
    };

    /**
     * Coordinate points
     */
    template<typename TYPE, int N>
    class CAxisPoint
    {
        TYPE m_ptPoint[N];

    public:
        // default constructor
        CAxisPoint()
        {
            static_assert(N > 1, "Coordinate dimension is illegal");
            SetSame(0);
        }

        // Constructor
        CAxisPoint(TYPE X_, TYPE Y_)
        {
            static_assert(N == 2, "Dimensions of coordinates do not match");
            m_ptPoint[axis::x] = X_;
            m_ptPoint[axis::y] = Y_;
        }

        CAxisPoint(TYPE X_, TYPE Y_, TYPE Z_)
        {
            static_assert(N == 3, "Dimensions of coordinates do not match");
            m_ptPoint[axis::x] = X_;
            m_ptPoint[axis::y] = Y_;
            m_ptPoint[axis::z] = Z_;
        }

        // initialize the values of all axes to the same value
        void SetSame(TYPE value_)
        {
            for (int i = 0; i < N; ++i) {
                m_ptPoint[i] = value_;
            }
        }

        // access or modify values by index
        const TYPE& operator[](int nIndex_) const
        {
            NEAT_RAISE_IF(nIndex_ < 0 || nIndex_ >= N, g_strSubscriptOutOfRange);
            return m_ptPoint[nIndex_];
        }

        TYPE& operator[](int nIndex_)
        {
            NEAT_RAISE_IF(nIndex_ < 0 || nIndex_ >= N, g_strSubscriptOutOfRange);
            return m_ptPoint[nIndex_];
        }

        // Arithmetic: += -= *= /= 
        CAxisPoint& operator+=(const CAxisPoint& ptPoint_)
        {
            static_assert(!std::is_same<TYPE, bool>::value, "This type does not support this operator");
            for (int i = 0; i < N; ++i) {
                m_ptPoint[i] += ptPoint_[i];
            }

            return *this;
        }

        CAxisPoint& operator-=(const CAxisPoint& ptPoint_)
        {
            static_assert(!std::is_same<TYPE, bool>::value, "This type does not support this operator");
            for (int i = 0; i < N; ++i) {
                m_ptPoint[i] -= ptPoint_[i];
            }

            return *this;
        }

        CAxisPoint& operator*=(TYPE nRatio_)
        {
            static_assert(!std::is_same<TYPE, bool>::value, "This type does not support this operator");
            for (int i = 0; i < N; ++i) {
                m_ptPoint[i] *= nRatio_;
            }

            return *this;
        }

        CAxisPoint& operator/=(TYPE nRatio_)
        {
            static_assert(!std::is_same<TYPE, bool>::value, "This type does not support this operator");
            BODOR_RAISE_IF(nRatio_ == 0.0, g_strDivisorEqualZero);
            for (int i = 0; i < N; ++i) {
                m_ptPoint[i] /= nRatio_;
            }

            return *this;
        }

        // Point-to-origin length: only supports 2D and 3D Cartesian coordinate systems
        double GetLength() const;

        // The square of the length of the point to the origin: only supports 2D and 3D Cartesian coordinate systems
        double GetSquareLength() const;
    };

    //////////////////////////////////////////////////////////////////////////
    // Arithmetic: + - * / 
    template<class TYPE, int N>
    CAxisPoint<TYPE, N> operator+(const CAxisPoint<TYPE, N>& ptLPoint_, const CAxisPoint<TYPE, N>& ptRPoint_)
    {
        return CAxisPoint<TYPE, N>(ptLPoint_) += ptRPoint_;
    }

    template<class TYPE, int N>
    CAxisPoint<TYPE, N> operator-(const CAxisPoint<TYPE, N>& ptLPoint_, const CAxisPoint<TYPE, N>& ptRPoint_)
    {
        return CAxisPoint<TYPE, N>(ptLPoint_) -= ptRPoint_;
    }

    template<class TYPE, int N>
    CAxisPoint<TYPE, N> operator*(const CAxisPoint<TYPE, N>& ptPoint_, TYPE nRatio_)
    {
        return CAxisPoint<TYPE, N>(ptPoint_) *= nRatio_;
    }

    template<class TYPE, int N>
    CAxisPoint<TYPE, N> operator*(TYPE nRatio_, const CAxisPoint<TYPE, N>& ptPoint_)
    {
        return CAxisPoint<TYPE, N>(ptPoint_) *= nRatio_;
    }

    template<class TYPE, int N>
    CAxisPoint<TYPE, N> operator/(const CAxisPoint<TYPE, N>& ptPoint_, TYPE nRatio_)
    {
        return CAxisPoint<TYPE, N>(ptPoint_) /= nRatio_;
    }

} // End namespace neat

#endif // __NEAT_POINT_H_20200127__