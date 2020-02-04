/***************************************************************************************************
 Created on: 2020-01-27
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatCore Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_ERRORID_H_20200127__
#define __NEAT_ERRORID_H_20200127__

#include <string>

/**
 * This file defines the error message ID.
 * The external program can replace the description of the resource file based on the ID.
 */
namespace neat
{
    /**
     * Array or container subscript out of range
     */
    const std::string g_strSubscriptOutOfRange = "ID_CORE_SUBSCRIPT_OUT_OF_RANGE";

    /**
     * Divisor is zero
     */
    const std::string g_strDivisorEqualZero = "ID_CORE_DIVISOR_EQUAL_ZERO";

    /**
     * Illegal arc bulge
     */
    const std::string g_strIllegalArcBulge = "ID_CORE_ILLEGAL_ARC_BULGE";

} // End namespace neat

#endif // __NEAT_ERRORID_H_20200127__