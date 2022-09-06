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

#ifndef _SSTEST_UTILITY_H_
#define _SSTEST_UTILITY_H_

#include <vector>
#include <utility>
#include "sstest_traits.h"

/**
 * \file sstest_utlility.h
 * \brief Helpful functions that aren't in standard C++11
 * 
 */

namespace sstest
{
    
    template <typename T1, typename T2>
    constexpr auto min(T1&& lhs, T2&& rhs) -> decltype(std::forward<T1>(lhs) < std::forward<T2>(rhs) ? std::forward<T1>(lhs) : std::forward<T2>(rhs))
    {
        return std::forward<T1>(lhs) < std::forward<T2>(rhs) ? std::forward<T1>(lhs) : std::forward<T2>(rhs);
    }

    template <typename T1, typename T2, typename ...Types>
    constexpr auto min(T1&& first, T2&& next, Types&&... rest) -> decltype(std::forward<T1>(first) < std::forward<T2>(next) ? min(std::forward<T1>(first), std::forward<Types>(rest)...) : min(std::forward<T2>(next), std::forward<Types>(rest)...))
    {
        return std::forward<T1>(first) < std::forward<T2>(next) ? min(std::forward<T1>(first), std::forward<Types>(rest)...) : min(std::forward<T2>(next), std::forward<Types>(rest)...);
    }

    template <typename T1, typename T2>
    constexpr auto max(T1&& lhs, T2&& rhs) -> decltype(std::forward<T1>(lhs) > std::forward<T2>(rhs) ? std::forward<T1>(lhs) : std::forward<T2>(rhs))
    {
        return std::forward<T1>(lhs) > std::forward<T2>(rhs) ? std::forward<T1>(lhs) : std::forward<T2>(rhs);
    }

    template <typename T1, typename T2, typename ...Types>
    constexpr auto max(T1&& first, T2&& next, Types&&... rest) -> decltype(std::forward<T1>(first) > std::forward<T2>(next) ? max(std::forward<T1>(first), std::forward<Types>(rest)...) : max(std::forward<T2>(next), std::forward<Types>(rest)...))
    {
        return std::forward<T1>(first) > std::forward<T2>(next) ? max(std::forward<T1>(first), std::forward<Types>(rest)...) : max(std::forward<T2>(next), std::forward<Types>(rest)...);
    }

    // TODO filter can't be generalized easily as there is no common insert in STL containters.
    template <typename T, typename Pred>
    std::vector<T> filter(const std::vector<T>& vec, const Pred& pred)
    {
        std::vector<T> rvec;
        for (const T& t : vec)
        {
            if (pred(t))
            {
                rvec.push_back(t);
            }
        }
        return rvec;
    }

    template <typename T, template <typename...> class Container, typename MapFunc>
    void map(Container<T>& vec, MapFunc func)
    {
        Container<T> rvec;
        for (T& t : vec)
        {
            t = func(t);
        }
        return rvec;
    }
}

#endif // _SSTEST_UTILITY_H_