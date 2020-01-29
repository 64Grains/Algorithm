/***************************************************************************************************
 Created on: 2020-01-27
 Created by: dquan2007@163.com
 Copyright (c) 2020-2020

 This file is part of C++ NeatCore Library.

 This library is free software; you can redistribute it and/or modify it under the terms of the GNU
 Lesser General Public License version 3 as published by the Free Software Foundation, with special
 exception defined in the file LICENSE.

***************************************************************************************************/
#ifndef __NEAT_MACRO_H_20200127__
#define __NEAT_MACRO_H_20200127__

#include <string>
#include <stdexcept>

/**
 * This file defines the preprocessor macros necessary to properly compile the NeatCore library.
 */
namespace neat
{
    /**
     * Maximum and minimum macros
     */
#ifndef max
    #define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
    #define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max3
    #define max3(a, b, c) (max(max(a, b), c))
#endif

#ifndef min3
    #define min3(a, b, c) (min(min(a, b), c))
#endif

/**
 * It is assumed here that the first parameter value of the __CUT macro is smaller than the second parameter value,
 * otherwise the result is unpredictable.
 */
#ifndef __CUT
    #define __CUT(x, low, up) ((x) = min(x, up), (x) = max(x, low))
#endif

    /**
     * An exception macro with a switch.
     * You can switch this condition macro according to the macro definition.
     * We tend to turn this macro off when compiling the Release version in order to improve the efficiency of the program,
     * but there is a risk that the exception is not thrown in time, and the program's behavior may unknowable.
     */
#ifndef NO_EXCEPTION_FOR_NEAT
    #define NEAT_RAISE_IF(CONDITION, MESSAGE) \
        if (CONDITION) { \
            char _szBuffer[64] = { '\0' }; \
            _ultoa_s(__LINE__, _szBuffer, 10); \
            std::string _strMsg = "Error(File " + std::string(__FILE__) + " - "; \
            _strMsg += "Line " + std::string(_szBuffer) + "): "; \
            throw std::domain_error(_strMsg + MESSAGE); \
        }
#else
    #define NEAT_RAISE_IF(CONDITION, MESSAGE)
#endif

#ifndef NEAT_RAISE
    #define NEAT_RAISE(CONDITION, MESSAGE) \
        if (CONDITION) { \
            char _szBuffer[64] = { '\0' }; \
            _ultoa_s(__LINE__, _szBuffer, 10); \
            std::string _strMsg = "Error(File " + std::string(__FILE__) + " - "; \
            _strMsg += "Line " + std::string(_szBuffer) + "): "; \
            throw std::domain_error(_strMsg + MESSAGE); \
        }
#endif

} // End namespace neat

#endif // __NEAT_MACRO_H_20200127__