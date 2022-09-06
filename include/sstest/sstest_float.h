/***************************************************************************//**
* SSTest: A C++ Testing Library
* 
* This software is distributed free of charge, under the MIT License.
* 
* 
* Copyright (c) 2022 David Lu
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal 
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
* SOFTWARE.
* 
*******************************************************************************/

#ifndef _SSTEST_FLOAT_H_
#define _SSTEST_FLOAT_H_

#include <cmath>
#include <limits>
#include <algorithm>
#include <string>
#include <cstdio>


/**
 * \file sstest_float.h
 * \brief Helper functions for floating point comparison
 * 
 */

namespace sstest
{
    template <typename F, typename = typename std::enable_if<std::is_floating_point<F>::value>::type>
    inline F rel_exp_eps(F f)
    {
        int exp = 0;
        std::frexp(f, &exp);
        if (!exp) return 0;
        return std::abs(std::pow(static_cast<F>(2.0), static_cast<F>(exp) - 1)) * std::numeric_limits<F>::epsilon();
    }

    template <typename F, typename = typename std::enable_if<std::is_floating_point<F>::value>::type>
    constexpr F rel_eps(F f)
    {
        return std::abs(f * std::numeric_limits<F>::epsilon());
    }

    /**
     * \brief Compare two floating point values and see if they are equal within a reasonable margin of error
     * 
     * \tparam F Floating point type
     * \param lhs First float to compare
     * \param rhs Second float to compare
     * \param units How many units of epsilon to compare with, by default this is 4
     * \return true if the difference between two floats is within margin of error
     * \return false else
     */
    template <typename F, typename = typename std::enable_if<std::is_floating_point<F>::value>::type>
    inline bool float_equal(F lhs, F rhs, size_t units)
    {
        return std::abs(rhs - lhs) <= (static_cast<F>(units) * std::max(rel_exp_eps(lhs), rel_exp_eps(rhs)));
    }

    template <typename F, typename = typename std::enable_if<std::is_floating_point<F>::value>::type>
    inline bool float_equal(F lhs, F rhs)//, size_t units = 4)
    {
        return float_equal(lhs, rhs, 4);
    }

    // digits10 + 2 is usually enough to spot first any difference due to rounding
    template <typename F, typename = typename std::enable_if<std::is_floating_point<F>::value>::type>
    inline std::string float_to_string(F f, size_t units = std::numeric_limits<F>::digits10 + 2)
    {
        std::string s;
        const std::string format_str = "%." + std::to_string(units) + "f";
        int buf_size = snprintf(nullptr, 0, format_str.c_str(), f);
        s.resize(static_cast<size_t>(buf_size) + 1); //  +1 for null char
#   ifdef _MSC_VER
#       pragma warning(push)
#       pragma warning(disable : 4996) // CRT safety warning for sprintf: we just got buffer size form snprintf
#   endif
        sprintf(&s[0], format_str.c_str(), f);
#   ifdef _MSC_VER
#       pragma warning(pop)
#   endif
        return s;
    }
}

#endif //_SSTEST_FLOAT_H_