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

#ifndef _SSTEST_TRAITS_H_
#define _SSTEST_TRAITS_H_

#include <type_traits>
#include <utility>
#include <iterator>

#include "sstest_def.h"

/**
 * \file sstest_traits.h
 * \brief Compile time metaprogramming extensions from standard library type_traits
 * 
 */

namespace sstest
{
    /**
     * \brief Helper SFINAE types for compile type metaprogramming that are not included in standard library
     * 
     */

    // void_t is in C++14, but not C++11
    template <typename... >
    using void_t = void;
            
    template <typename C, typename = void>
    struct is_iterable : std::false_type 
    {};

    template <typename C>
    struct is_iterable <C, void_t<decltype(std::declval<C&>().begin()),
                                 decltype(std::declval<C&>().end()),
                                 decltype(++(std::declval<C&>().begin())),
                                 decltype(std::declval<C&>().begin() == std::declval<C&>().end())>>
        : std::true_type 
    {};

    template <typename C, typename = void>
    struct is_container : std::false_type 
    {};

    template <typename C>
    struct is_container <C, void_t<decltype(std::declval<C&>().begin()),
                                 decltype(std::declval<C&>().end()),
                                 decltype(++(std::declval<C&>().begin())),
                                 decltype(std::declval<C&>().begin() == std::declval<C&>().end()),
                                 decltype(*(std::declval<C&>().begin()))>>
        : std::true_type 
    {};

    template <typename N, typename = void>
    struct is_comparable : std::false_type
    {};

    template <typename N>
    struct is_comparable <N, void_t<decltype(std::declval<N&>() < std::declval<N&>()),
                                   decltype(std::declval<N&>() > std::declval<N&>()),
                                   decltype(std::declval<N&>() == std::declval<N&>())>>
        : std::true_type
    {};

    template <typename T, typename = void>
    struct is_complete_type : std::false_type
    {};

    template <typename T>
    struct is_complete_type <T, void_t<decltype(sizeof(T))>>
        : std::true_type
    {};

    template <typename N>
    struct range
    {
        constexpr range(const N& lower, const N& upper) noexcept
            : lower(lower), upper(upper)
        { }
        // TODO includsive/excl. boudns
        constexpr bool in_range(const N& n) const noexcept
        {
            return (n >= lower) && (n <= upper);
        }

        template <typename NEQ>
        friend constexpr bool operator==(const range<NEQ>& lhs, const range<NEQ>& rhs);

        N lower;
        N upper;
    };

    template <typename N>
    constexpr bool operator==(const range<N>& lhs, const range<N>& rhs)
    {
        return (lhs.lower == rhs.lower) && (lhs.upper == rhs.upper);
    }

    template <typename N>
    struct iterable_range : range<N>
    {
        // TODO includsive/excl. boudns
        class const_iterator : public std::iterator<
                                                    std::forward_iterator_tag,
                                                    const N,
                                                    size_t,
                                                    const N*,
                                                    const N
                                                    > 
        {
        public:
            explicit const_iterator(const iterable_range<N>& range, N begin) noexcept
                : range(range), current(begin) 
            {
                if (!range.in_range(begin))
                {
                    current = range.lower;
                }
            }

            const_iterator& operator++() noexcept
            {
                ++current;
                return *this;
            }

            bool operator==(const const_iterator& other) const noexcept
            {
                return (this->range == other.range) && (this->current == other.current);
            }

            bool operator!=(const const_iterator& other) const noexcept
            {
                return !(*this == other);
            }

            const N operator*() const noexcept
            { 
                return current;
            }

        private:
            const iterable_range<N>& range;
            N current;
        };
        
        using const_reverse_iterator = std::reverse_iterator<const_iterator> ;
        using iterator = const_iterator;
        using reverse_iterator = const_reverse_iterator;

        constexpr iterable_range(const N& lower, const N& upper) noexcept
            : range<N>(lower, upper)
        { }

        constexpr const_iterator begin() const noexcept
        {
            return const_iterator(*this, this->lower);
        }

        constexpr const_iterator end() const noexcept
        {
            return const_iterator(*this, this->upper);
        }

        
    };

    // impl. from https://stackoverflow.com/questions/12030538/calling-a-function-for-each-variadic-template-argument-and-an-array
    template <typename F, typename... Args>
    void for_each_template_arg(F f, Args&& ...args)
    {
        [](...){}((f(std::forward<Args>(args)), 0)...);
    }
}

#endif // _SSTEST_TRAITS_H_