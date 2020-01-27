/***************************************************************************************************
 Created on: 2020-01-27
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatCore Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_FAST_VECTOR_H_20200127__
#define __NEAT_FAST_VECTOR_H_20200127__

#include <vector>

/**
 * This file defines a container similar to std::vector, which retains the elements in std::vector container for memory reuse.
 */
namespace neat
{
    /**
     * A custom fasr container of type std::vector.
     * This container retains the elements in std::vector and controls the state of the fast container by manipulating the index.
     */
    template<typename TYPE>
    class CFastVector
    {
    public:
        typedef typename std::vector<TYPE>::size_type size_type; typedef typename std::vector<TYPE>::size_type size_type;

        CFastVector()
            : m_nStartIndex(0)
            , m_nEndIndex(0)
        {
            m_vecDatas.resize(10);
        }

        size_type size() const { return m_nEndIndex - m_nStartIndex; }
        bool empty() const { return size() == 0; }

        const TYPE& operator[](size_type nIndex_) const
        {
            NEAT_RAISE_IF(size() <= nIndex_, g_strSubscriptOutOfRange);
            return m_vecDatas[m_nStartIndex + nIndex_];
        }

        TYPE& operator[](size_type nIndex_)
        {
            NEAT_RAISE_IF(size() <= nIndex_, g_strSubscriptOutOfRange);
            return m_vecDatas[m_nStartIndex + nIndex_];
        }

        const TYPE& front() const
        {
            NEAT_RAISE_IF(empty(), g_strSubscriptOutOfRange);
            return m_vecDatas[m_nStartIndex];
        }

        TYPE& front()
        {
            NEAT_RAISE_IF(empty(), g_strSubscriptOutOfRange);
            return m_vecDatas[m_nStartIndex];
        }

        const TYPE& back() const
        {
            NEAT_RAISE_IF(empty(), g_strSubscriptOutOfRange);
            return m_vecDatas[m_nEndIndex - 1];
        }

        TYPE& back()
        {
            NEAT_RAISE_IF(empty(), g_strSubscriptOutOfRange);
            return m_vecDatas[m_nEndIndex - 1];
        }

        void push_back(const TYPE& value_)
        {
            NEAT_RAISE_IF(m_nEndIndex > m_vecDatas.size(), g_strSubscriptOutOfRange);
            if (m_nEndIndex == m_vecDatas.size()) {
                m_vecDatas.push_back(value_);
            }
            else {
                m_vecDatas[m_nEndIndex] = value_;
            }

            ++m_nEndIndex;
        }

        void pop_back()
        {
            NEAT_RAISE_IF(empty() || m_nEndIndex == 0, g_strSubscriptOutOfRange);
            --m_nEndIndex;
        }

        void pop_front()
        {
            NEAT_RAISE_IF(empty(), g_strSubscriptOutOfRange);
            ++m_nStartIndex;
        }

        void clear()
        {
            m_nStartIndex = 0;
            m_nEndIndex = 0;
        }

        void resize(size_type nSize_)
        {
            m_nStartIndex = 0;
            m_nEndIndex = m_nStartIndex + nSize_;
            if (m_vecDatas.size() < nSize_) {
                m_vecDatas.resize(nSize_);
            }
        }

        void resize(size_type nSize_, const TYPE& nValue_)
        {
            resize(nSize_);
            for (size_type i = 0; i < nSize_; ++i) {
                m_vecDatas[i] = nValue_;
            }
        }

        void copy(const CFastVector& vecDatas_)
        {
            m_vecDatas.reserve(m_vecDatas.size() + vecDatas_.size());
            for (size_type i = 0, _nSize = vecDatas_.size(); i < _nSize; ++i) {
                push_back(vecDatas_[i]);
            }
        }

    private:
        std::vector<TYPE> m_vecDatas;
        size_type m_nStartIndex;
        size_type m_nEndIndex;
    };

} // End namespace neat

#endif // __NEAT_FAST_VECTOR_H_20200127__