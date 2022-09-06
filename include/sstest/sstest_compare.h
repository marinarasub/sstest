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

#ifndef _SSTEST_COMPARE_H_
#define _SSTEST_COMPARE_H_

#include <functional>
#include <tuple>
#include <utility>
#include <type_traits>

#include "sstest_float.h"


/**
 * \file sstest_compare.h
 * \brief Contains all comparison and related helpers for 
 * Because all template parameters are deduced seperately per argument, comparing across types (such as pointer type and integral NULL) may give compiler warnings
 * that would not be present with implicit type conversion
 */

namespace sstest
{

    /**
     * \brief Contains helpers for performing comparisons while storing evaluated results
     * \note That implementation uses references, so some comparisons may differ slighly than if you were to use built-in operators directly 
     * - e.g. comparing to NULL macro becomes and integer type (which may induce compiler warning/error)
     * \note All comparisons must return bool (or be convertible to bool)
     * Unlike C++ standard library equal_to etc, the types compared can be different, template parameters can be inferred, and non-const references can be used.
     */
    namespace comparison
    {
        // constant bool return value since CompareHelper (below) requires predicate in constructor

        template <typename... Types>
        constexpr bool always_true(const Types&...)
        {
            return true;
        }

        template <typename Types>
        constexpr bool always_false(const Types&...)
        {
            return false;
        }

        /////////////////////////////////////////////////////////////////////////////////////
        // constexpr compare helpers to be able to use binary operators as first class object
        /////////////////////////////////////////////////////////////////////////////////////
        
        // 0. identity, return ref to itself, so can be passed as predicate argument
        template <typename T>
        constexpr auto identity(T&& arg) -> decltype(std::forward<T>(arg))
        {
            return std::forward<T>(arg);
        }

        // 0b. negation using operator!
        template <typename T>
        constexpr auto negate(T&& arg) -> decltype(!std::forward<T>(arg))
        {
            return !std::forward<T>(arg);
        }

        // 1. equality comparison, uses operator ==. return true if == returns true
        
        // 1a. symmetric operator ==
        template <typename T1, typename T2>
        constexpr auto equal(T1&& lhs, T2&& rhs) -> decltype((std::forward<T1>(lhs) == std::forward<T2>(rhs)))
        {
            return (std::forward<T1>(lhs) == std::forward<T2>(rhs));
        }
        struct equal_object
        {
            template <typename T1, typename T2>
            constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs)))
            {
                return equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs));
            }
        };
        // 1b. asymmetric comparison. If operator == is not symmetric and you want to compare 
        // both ways, use this one
        template <typename T1, typename T2>
        constexpr auto equal_asymmetric(T1&& lhs, T2&& rhs) -> decltype((std::forward<T1>(lhs) == std::forward<T2>(rhs)) &&
                                                                        (std::forward<T1>(rhs) == std::forward<T2>(lhs)))
        {
            return 
                (std::forward<T1>(lhs) == std::forward<T2>(rhs)) && 
                (std::forward<T1>(rhs) == std::forward<T2>(lhs));
        }
        struct equal_asymmetric_object
        {
            template <typename T1, typename T2>
            constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(equal_asymmetric<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs)))
            {
                return equal_asymmetric<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs));
            }
        };
        
        ///////////////////////////////////////////

        // 2. inequality comparison, uses operator !=. return true if != returns true

        // 2a. symmetric operator !=
        template <typename T1, typename T2>
        constexpr auto not_equal(T1&& lhs, T2&& rhs) -> decltype((std::forward<T1>(lhs) != std::forward<T2>(rhs)))
        {
            return (std::forward<T1>(lhs) != std::forward<T2>(rhs));
        }
        struct not_equal_object
        {
            template <typename T1, typename T2>
            constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(not_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs)))
            {
                return not_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs));
            }
        };

        // 2b. asymmetric comparison. If operator != is not symmetric and you want to compare 
        // both ways, use this one
        template <typename T1, typename T2>
        constexpr auto not_equal_asymmetric(T1&& lhs, T2&& rhs) -> decltype((std::forward<T1>(lhs) != std::forward<T2>(rhs)) &&
                                                                            (std::forward<T1>(rhs) != std::forward<T2>(lhs)))
        {
            return
                (std::forward<T1>(lhs) != std::forward<T2>(rhs)) &&
                (std::forward<T1>(rhs) != std::forward<T2>(lhs));
        }
        struct not_equal_asymmetric_object
        {
            template <typename T1, typename T2>
            constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(not_equal_asymmetric<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs)))
            {
                return not_equal_asymmetric<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs));
            }
        };

        ///////////////////////////////////////////

        // 3. strictly less than comparison, uses operator <. return true if left side < right side

        template <typename T1, typename T2>
        constexpr auto less(T1&& lhs, T2&& rhs) -> decltype((std::forward<T1>(lhs) < std::forward<T2>(rhs)))
        {
            return (std::forward<T1>(lhs) < std::forward<T2>(rhs));
        }
        struct less_object
        {
            template <typename T1, typename T2>
            constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(less<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs)))
            {
                return less<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs));
            }
        };

        ///////////////////////////////////////////

        // 4. strictly greater than comparison, uses operator >. return true if left side > right side

        template <typename T1, typename T2>
        constexpr auto greater(T1&& lhs, T2&& rhs) -> decltype((std::forward<T1>(lhs) > std::forward<T2>(rhs)))
        {
            return (std::forward<T1>(lhs) > std::forward<T2>(rhs));
        }
        struct greater_object
        {
            template <typename T1, typename T2>
            constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(greater<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs)))
            {
                return greater<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs));
            }
        };

        ///////////////////////////////////////////

        // 5. equal or less than comparison, uses operator <=. return true if left side <= right side

        template <typename T1, typename T2>
        constexpr auto less_equal(T1&& lhs, T2&& rhs) -> decltype((std::forward<T1>(lhs) <= std::forward<T2>(rhs)))
        {
            return (std::forward<T1>(lhs) <= std::forward<T2>(rhs));
        }
        struct less_equal_object
        {
            template <typename T1, typename T2>
            constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(less_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs)))
            {
                return less_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs));
            }
        };

        ///////////////////////////////////////////

        // 6. equal or greater than comparison, uses operator >=. return true if left side >= right side

        template <typename T1, typename T2>
        constexpr auto greater_equal(T1&& lhs, T2&& rhs) -> decltype((std::forward<T1>(lhs) >= std::forward<T2>(rhs)))
        {
            return (std::forward<T1>(lhs) >= std::forward<T2>(rhs));
        }
        struct greater_equal_object
        {
            template <typename T1, typename T2>
            constexpr auto operator()(T1&& lhs, T2&& rhs) const -> decltype(greater_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs)))
            {
                return greater_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs));
            }
        };

        ///////////////////////////////////
        
        // 7. all - return true if all values are true 
        // O(n) in number of args

        // base case - return true if single element is true
        template <typename T>
        constexpr bool all(T&& t)
        {
            return bool(std::forward<T>(t));
        }
        // else check first element and recusively check rest (short circuit if first is false)
        template <typename T1, typename T2, typename... Types>
        constexpr bool all(T1&& first, T2&& next, Types&&... rest)
        {
            return all(std::forward<T1>(first)) && all(std::forward<T2>(next), std::forward<Types>(rest)...);
        }

        ///////////////////////////////////
        
        // 8. any - return true if any argument given is true

        // base case - return true if single element if true
        template <typename T>
        constexpr bool any(T&& t)
        {
            return bool(std::forward<T>(t));
        }
        // else check first element and rest if any are true
        template <typename T1, typename T2, typename... Types>
        constexpr bool any(T1&& first, T2&& next, Types&&... rest)
        {
            return any(std::forward<T1>(first)) || any(std::forward<T2>(next), std::forward<Types>(rest)...);
        }

        //////////////////////////////////

        // 9. approximate comparison, return true if difference is smaller than supplied delta

        template <typename T1, typename T2, typename D>
        constexpr bool approx_equal(T1&& lhs, T2&& rhs, D&& del)
        {
            return std::abs(rhs - lhs) <= del;
        }
        struct approx_equal_object
        {
            template <typename T1, typename T2, typename D>
            constexpr bool operator()(T1&& lhs, T2&& rhs, D&& del) const
            {
                return approx_equal<T1, T2, D>(std::forward<T1>(lhs), std::forward<T2>(rhs), std::forward<D>(del));
            }
        };

        //////////////////////////////////

        namespace internal
        {
            ///////////////////////////////////////////////////////
            // Helpers for compring variadic number or args
            //////////////////////////////////////////////////////
             
            // 1. binary comparison each element to first element
            // O(n) in number of args

            // base case, 1 arg - vacuously true
            template <typename Pred, typename T>
            constexpr bool binary_ncompare_first(const Pred&, T&&)
            {
                return true;
            }
            // else compare between at least next arg in list to first element
            template <typename Pred, typename T1, typename T2, typename... Types>
            constexpr bool binary_ncompare_first(const Pred& pred, T1&& first, T2&& next, Types&&... rest)
            {
                return pred(std::forward<T1>(first), std::forward<T2>(next)) && 
                    binary_ncompare_first(pred, std::forward<T1>(first), std::forward<Types>(rest)...);
            }

            //////////////////////////

            // 2. binary comparison each element to adjacent
            // O(n) in number of args
            
            // base case, 1 arg - vacuously true
            template <typename Pred, typename T>
            constexpr bool binary_ncompare_adjacent(const Pred&, T&&)
            {
                return true;
            }
            // else compare between at least next arg in list to previous element
            template <typename Pred, typename T1, typename T2, typename... Types>
            constexpr bool binary_ncompare_adjacent(const Pred& pred, T1&& first, T2&& next, Types&&... rest)
            {
                return pred(std::forward<T1>(first), std::forward<T2>(next)) &&
                    binary_ncompare_adjacent(pred, std::forward<T2>(next), std::forward<Types>(rest)...);
            }

            ///////////////////////////////

            // 3. binary compare between each element to each other element
            // O(n^2) in number of args

            // base case, 1 arg - vacuously true
            template <typename Pred, typename T>
            constexpr bool binary_ncompare_each(const Pred&, T&&)
            {
                return true;
            }
            // else call binary compare first with current args, then recursively call itself for for rest to compare against next arg.
            template <typename Pred, typename T1, typename T2, typename... Types>
            constexpr bool binary_ncompare_each(const Pred& pred, T1&& first, T2&& next, Types&&... rest)
            {
                return binary_ncompare_first(pred, std::forward<T1>(first), std::forward<T2>(next), std::forward<Types>(rest)...) &&
                    binary_ncompare_each(pred, std::forward<T2>(next), std::forward<Types>(rest)...);
            }

            ///////////////////////////////////

        }

        ////////////////////////////////////////////////////////////////
        // variadic comparison helpers
        ////////////////////////////////////////////////////////////////

        // 1. all equal first element
        // return true if all items are equal to each other, symmetric
        template <typename T1, typename T2, typename... Types>
        constexpr bool all_equal_first(T1&& first, T2&& next, Types&&... rest)
        {
            return internal::binary_ncompare_first(equal_object(), std::forward<T1>(first), std::forward<T2>(next), std::forward<Types>(rest)...);
        }

        // 1. all equal each other element
        // return true if all items are equal to each other, symmetric
        template <typename T1, typename T2, typename... Types>
        constexpr bool all_equal_each(T1&& first, T2&& next, Types&&... rest)
        {
            return internal::binary_ncompare_each(equal_object(), std::forward<T1>(first), std::forward<T2>(next), std::forward<Types>(rest)...);
        }

        // return true if arguments are arranged in ascending order, that is first <= next <= rest...
        template <typename T1, typename T2, typename... Types>
        constexpr bool ascending(T1&& first, T2&& next, Types&&... rest)
        {
            return internal::binary_ncompare_adjacent(less_equal_object(), std::forward<T1>(first), std::forward<T2>(next), std::forward<Types>(rest)...);
        }

        // return true if arguments are arranged in descending order, that is first >= next >= rest...
        template <typename T1, typename T2, typename... Types>
        constexpr bool descending(T1&& first, T2&& next, Types&&... rest)
        {
            return internal::binary_ncompare_adjacent(greater_equal_object(), std::forward<T1>(first), std::forward<T2>(next), std::forward<Types>(rest)...);
        }

        // return true if arguments are arranged in ascending order, that is first < next < rest...
        template <typename T1, typename T2, typename... Types>
        constexpr bool strictly_ascending(T1&& first, T2&& next, Types&&... rest)
        {
            return internal::binary_ncompare_adjacent(less_object(), std::forward<T1>(first), std::forward<T2>(next), std::forward<Types>(rest)...);
        }

        // return true if arguments are arranged in descending order, that is first > next > rest...
        template <typename T1, typename T2, typename... Types>
        constexpr bool strictly_descending(const T1& first, const T2& next, const Types&... rest)
        {
            return internal::binary_ncompare_adjacent(greater_object(), first, next, rest...);
        }

      
        // helpers since built in types etc dont have operators as function pointers
        // compare helpers are defined so macros dont repeat arguemtns when comparing and storing values
        // ie ASSERT(dynamicFunction()) expands to EvalAndPrint(dynamicFunction(), dynamicFunction()) may result in unwanted or undefined behavior

        /**
        * \warning DUE TO HOLDING REFERENCES, IT IS NOT SAFE TO
        */
        template <typename Res, typename...Types>
        struct CompareHelper
        {

            template <typename, typename...> friend struct CompareHelper;

            template <typename Pred>
            constexpr CompareHelper(const Pred& pred, Types&&... types) noexcept( noexcept(std::declval<Pred>() (std::declval<Types>()...) ) )
                : CompareHelper(pred(std::forward<Types>(types)...), std::tuple<const Types&...>(types...) )
            {}

            void operator=(CompareHelper&&) = delete;
            void operator=(const CompareHelper&) = delete;

            constexpr CompareHelper(CompareHelper&& other) noexcept
                : result(std::forward<Res>(other.result)), args(std::move(other.args))
            {}

        public:

            operator bool()
            {
                static_assert(std::is_convertible<Res, bool>::value, "comparison result type must be convertible to bool");
                return bool(std::forward<Res>(result));
            }


        protected:

            /**
             * \brief Direct constructor creates CompareHelper given explicit bool result tuple of arguments
             * 
             */
            constexpr CompareHelper(Res&& result, const std::tuple<const Types&...>& tuple) noexcept
                : result(std::forward<Res>(result)), args(tuple)
            {}

        public:
            Res result;
            const std::tuple<const Types&...> args;
        };

        //////// helpers for type deduction /////////

        template <typename T>
        constexpr auto make_compare(T&& result) -> CompareHelper<decltype(identity<T>(std::forward<T>(result))), T>
        {
            return CompareHelper<decltype(identity<T>(std::forward<T>(result))), T> (identity<T>, std::forward<T>(result));
        }

        template <typename T>
        constexpr auto make_negation_compare(T&& result) -> CompareHelper<decltype(negate<T>(std::forward<T>(result))), T>
        {
            return CompareHelper<decltype(negate<T>(std::forward<T>(result))), T>(negate<T>, std::forward<T>(result));
        }
        
        // nullptr_t literal can't be negated so we define specialization
        constexpr CompareHelper<bool, std::nullptr_t> make_negation_compare(std::nullptr_t)
        {
            return CompareHelper<bool, std::nullptr_t>(always_true<std::nullptr_t>, nullptr);
        }

        template <typename Pred, typename... Types>
        constexpr auto make_compare(const Pred& pred, Types&&... args) -> CompareHelper<decltype(pred(std::forward<Types>(args)...)), Types...>
        {
            return CompareHelper<decltype(pred(std::forward<Types>(args)...)), Types...> (pred, std::forward<Types>(args)...);
        }

        template <typename T1, typename T2>
        constexpr auto make_equal_compare(T1&& lhs, T2&& rhs) -> CompareHelper<decltype(equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2>
        {
            return CompareHelper<decltype(equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2> (equal<T1, T2>, std::forward<T1>(lhs), std::forward<T2>(rhs));
        }

        template <typename T1, typename T2>
        constexpr auto make_not_equal_compare(T1&& lhs, T2&& rhs) -> CompareHelper<decltype(not_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2>
        {
            return CompareHelper<decltype(not_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2> (not_equal<T1, T2>, std::forward<T1>(lhs), std::forward<T2>(rhs));
        }

        template <typename T1, typename T2>
        constexpr auto make_less_compare(T1&& lhs, T2&& rhs) -> CompareHelper<decltype(less<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2>
        {
            return CompareHelper<decltype(less<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2> (less<T1, T2>, std::forward<T1>(lhs), std::forward<T2>(rhs));
        }

        template <typename T1, typename T2>
        constexpr auto make_greater_compare(T1&& lhs, T2&& rhs) -> CompareHelper<decltype(greater<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2>
        {
            return CompareHelper<decltype(greater<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2> (greater<T1, T2>, std::forward<T1>(lhs), std::forward<T2>(rhs));
        }

        template <typename T1, typename T2>
        constexpr auto make_less_equal_compare(T1&& lhs, T2&& rhs) -> CompareHelper<decltype(less_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2>
        {
            return CompareHelper<decltype(less_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2> (less_equal<T1, T2>, std::forward<T1>(lhs), std::forward<T2>(rhs));
        }

        template <typename T1, typename T2>
        constexpr auto make_greater_equal_compare(T1&& lhs, T2&& rhs) -> CompareHelper<decltype(greater_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2>
        {
            return CompareHelper<decltype(greater_equal<T1, T2>(std::forward<T1>(lhs), std::forward<T2>(rhs))), T1, T2> (greater_equal<T1, T2>, std::forward<T1>(lhs), std::forward<T2>(rhs));
        }

        template <typename T1, typename T2, typename D>
        constexpr auto make_approx_equal_compare(T1&& lhs, T2&& rhs, D&& del) -> CompareHelper<decltype(approx_equal<T1, T2, D>(std::forward<T1>(lhs), std::forward<T2>(rhs), std::forward<D>(del))), T1, T2, D>
        {
            return CompareHelper<decltype(approx_equal<T1, T2, D>(std::forward<T1>(lhs), std::forward<T2>(rhs), std::forward<D>(del))), T1, T2, D> (approx_equal<T1, T2, D>, std::forward<T1>(lhs), std::forward<T2>(rhs), std::forward<D>(del));
        }

        template <typename... Types>
        constexpr auto make_all_compare(Types&&... args) -> CompareHelper<decltype(all<Types...>(std::forward<Types>(args)...)), Types...>
        {
            return CompareHelper<decltype(all<Types...>(std::forward<Types>(args)...)), Types...> (all<Types...>, std::forward<Types>(args)...);
        }

        template <typename... Types>
        constexpr auto make_any_compare(Types&&... args) -> CompareHelper<decltype(any<Types...>(std::forward<Types>(args)...)), Types...>
        {
            return CompareHelper<decltype(any<Types...>(std::forward<Types>(args)...)), Types...> (any<Types...>, std::forward<Types>(args)...);
        }

        template <typename... Types>
        constexpr auto make_all_equal_first_compare(Types&&... args) -> CompareHelper<decltype(all_equal_first<Types...>(std::forward<Types>(args)...)), Types...>
        {
            return CompareHelper<decltype(all_equal_first<Types...>(std::forward<Types>(args)...)), Types...> (all_equal_first<Types...>, std::forward<Types>(args)...);
        }

        template <typename... Types>
        constexpr auto make_all_equal_each_compare(Types&&... args) -> CompareHelper<decltype(all_equal_each<Types...>(std::forward<Types>(args)...)), Types...>
        {
            return CompareHelper<decltype(all_equal_each<Types...>(std::forward<Types>(args)...)), Types...> (all_equal_each<Types...>, std::forward<Types>(args)...);
        }

        template <typename... Types>
        constexpr auto make_ascending_compare(Types&&... args) -> CompareHelper<decltype(ascending<Types...>(std::forward<Types>(args)...)), Types...>
        {
            return CompareHelper<decltype(ascending<Types...>(std::forward<Types>(args)...)), Types...> (ascending<Types...>, std::forward<Types>(args)...);
        }

        template <typename... Types>
        constexpr auto make_descending_compare(Types&&... args) -> CompareHelper<decltype(descending<Types...>(std::forward<Types>(args)...)), Types...>
        {
            return CompareHelper<decltype(descending<Types...>(std::forward<Types>(args)...)), Types...> (descending<Types...>, std::forward<Types>(args)...);
        }

        template <typename F>
        constexpr auto make_float_equal_compare(F&& lhs, F&& rhs) -> CompareHelper<decltype(float_equal<F>(std::forward<F>(lhs), std::forward<F>(rhs))), F, F>
        {
            return CompareHelper<decltype(float_equal<F>(std::forward<F>(lhs), std::forward<F>(rhs))), F, F>( (bool(*)(F, F)) float_equal<F>, std::forward<F>(lhs), std::forward<F>(rhs));
        }

        ////////////////////////////////////////////////////////////////////////////////////////////////

        /**
         * \brief Evaluate result from a series of comparison operators, but also store individual arguments to each operator
         * Stores intermediate result of all operations, so subsuquent operations behave as expected semantically
         * Arguments stored are ordered by first evaluated to most recent
         * \note I must be copy construbtible to be able to store in class (storing const& or && will get destoryed in reurive eat)
         * \note The intermediate value must be stored as a non-reference or pointer type, so it will be passed by const& to the next operation,
         * which may not be consistent with intended behavior if the input types have overloads for both && and const&
         *
         * \tparam I Type of the intermediate value
         * \tparam Types Variadic types of each argument stored
         */
        template <typename I, typename... Types>
        struct Collector : public comparison::CompareHelper<I, Types...>
        {

            template <typename, typename...> friend struct Collector;
            friend struct Decomposer;

        protected:

            /**
             * \brief Direct constructor creates a collector with intermediate value and tuple of arguments, Does not evaluate the intermediate to bool
             * since the intermediate may not be convertible to bool
             * \sa complete()
             *
             */
            constexpr Collector(I&& val, const std::tuple<const Types&...>& tup)
                : comparison::CompareHelper<I, Types...>(std::forward<I>(val), tup) //, intermediate(val) //I(val))//std::forward<I>(val))//, args(tup)
            {
            }

            /**
             * \brief Helper function to chain on each operator result as well as the argument given to the operator
             *
             * \tparam I2 Type of the resulting intermediate
             * \tparam T Type of the new argument given to the operator function
             * \param food
             * \param t
             * \return constexpr Collector<I2, Types..., T>
             */
            template <typename I2, typename T>
            Collector<I2, Types..., T> eat(I2&& food, T&& t)
            {
                return Collector<I2, Types..., T>(std::forward<I2>(food), std::tuple_cat(this->args, std::tuple<const T&>(std::forward<T>(t))));
            }

        public:

            template <typename T>
            auto operator==(T&& t) -> decltype(this->eat(std::forward<I>(this->result) == std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) == std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator!=(T&& t) -> decltype(this->eat(std::forward<I>(this->result) != std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) != std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator<(T&& t) -> decltype(this->eat(std::forward<I>(this->result) < std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) < std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator>(T&& t) -> decltype(this->eat(std::forward<I>(this->result) > std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) > std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator<=(T&& t) -> decltype(this->eat(std::forward<I>(this->result) <= std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) <= std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator>=(T&& t) -> decltype(this->eat(std::forward<I>(this->result) >= std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) >= std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator<<(T&& t) -> decltype(this->eat(std::forward<I>(this->result) << std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) << std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator>>(T&& t) -> decltype(this->eat(std::forward<I>(this->result) >> std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) >> std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator|(T&& t) -> decltype(this->eat(std::forward<I>(this->result) | std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) | std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator&(T&& t) -> decltype(this->eat(std::forward<I>(this->result)& std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result)&std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator^(T&& t) -> decltype(this->eat(std::forward<I>(this->result) ^ std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) ^ std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator||(T&& t) -> decltype(this->eat(std::forward<I>(this->result) || std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) || std::forward<T>(t), std::forward<T>(t));
            }

            template <typename T>
            auto operator&&(T&& t) -> decltype(this->eat(std::forward<I>(this->result) && std::forward<T>(t), std::forward<T>(t)))
            {
                return this->eat(std::forward<I>(this->result) && std::forward<T>(t), std::forward<T>(t));
            }

#if (__cplusplus >= 202002L)

            template <typename T>
            constexpr auto operator<=>(T&& t) -> decltype(this->eat(std::forward<I>(this->result) <=> std::forward<T>(t), std::forward<I>(t)))
            {
                return this->eat(std::forward<I>(this->result) <=> std::forward<T>(t), std::forward<I>(t));
            }
#endif

        };

        /**
         * \brief Decomposer is a helper class for collecting the first argument in a comparison statement
         *
         * Usage:
         * \code {.cpp}
         * Decomposer() << <args...>
         * \endcode
         *
         */
        struct Decomposer
        {
            /**
             * \brief Stream operator to collect the first argument in a series of binary operation arguments with type deduction
             * and return apporpritate Collector type to continue operator chaining
             *
             * \sa Collector<...>
             * \tparam T
             * \param t
             * \return Collector<T, T> with t as the sole argument
             */
            template <typename T>
            constexpr auto operator<<(T&& t) const noexcept -> Collector<decltype(std::forward<T>(t)), T>
            {
                return Collector<decltype(std::forward<T>(t)), T> (std::forward<T>(t), std::tuple<const T&>(t));
            }
        };

    }


}

#endif // _SSTEST_COMPARE_H_