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

#ifndef _SSTEST_ASSERTION_H_
#define _SSTEST_ASSERTION_H_

#include <string>
#include <tuple>
#include <utility>
#include <type_traits>
#include "sstest_info.h"
#include "sstest_compare.h"
#include "sstest_config.h"
#include "sstest_string.h"


/**
 * \file sstest_assertion.h
 * \brief Contains definitions to be used for tracking SSTest assertions used by the end user
 * Also contains internal macro definitions for assertions
 * 
 */

namespace sstest
{    

    enum class AssertionResult
    {
        INVALID = -1,
        FAIL = 0,
        SUCCESS = 1,
        PASS = SUCCESS,
    };

    /**
     * \brief Lighweight assertion class that contains comparison result and information about the type & location of asseriton ran
     * 
     * \tparam Types arguments stored by a CompareHelper type
     * \note Since this class relies on compare helper, the assertion class must be used before the end of its containing full expression,
     * or argument reference through CompareHelper will become dangling references.
     */
    template <typename... Types>
    class Assertion
    {

    public:

        constexpr Assertion(TestInfo test_info, LineInfo line_info, StringView arg_text, comparison::CompareHelper<Types...>&& compare) noexcept
            : compare_(std::forward<comparison::CompareHelper<Types...>>(compare)), assertion_info(test_info), line_info(line_info), arg_text(arg_text)
        {
        }

        constexpr operator bool() const noexcept
        {
            return passed();
        }

        constexpr bool operator!() const noexcept
        {
            return !passed();
        }

        constexpr AssertionResult result() const noexcept
        {
            return compare_ ? AssertionResult::PASS : AssertionResult::FAIL;
        }

        constexpr const comparison::CompareHelper<Types...>& compare_result() const noexcept
        {
            return compare_;
        }

        inline std::string where() const noexcept
        {
            return (line_info.where());
        }

        inline std::string text() const noexcept
        {
            return (std::string(assertion_info.name) + (arg_text ? " ( " + std::string(arg_text) + " ) " : ""));
        }

        constexpr bool passed() const noexcept
        {
            return (result() == AssertionResult::SUCCESS);
        }

        constexpr bool failed() const noexcept
        {
            return (result() == AssertionResult::FAIL);
        }

    private:
        comparison::CompareHelper<Types...>&& compare_;
        TestInfo assertion_info;
        LineInfo line_info;
        StringView arg_text;
        
    };

    /**
     * \brief Partial specialization for assertion with only a bool.
     */
    template <>
    class Assertion<bool>
    {
    public:

        constexpr Assertion() noexcept
            : result_(AssertionResult::INVALID), assertion_info(TestInfo("")), line_info(LineInfo("", 0)), arg_text("")
        {
        }

        constexpr Assertion(TestInfo test_info, LineInfo line_info, StringView arg_text, bool eval) noexcept
            : result_(eval ? AssertionResult::PASS : AssertionResult::FAIL), assertion_info(test_info), line_info(line_info), arg_text(arg_text)
        {
        }

        constexpr operator bool() const noexcept
        {
            return passed();
        }

        constexpr bool operator!() const noexcept
        {
            return !passed();
        }

        constexpr AssertionResult result() const noexcept
        {
            return result_;
        }

        inline std::string where() const noexcept
        {
            return (line_info.where());
        }

        inline std::string text() const noexcept
        {
            return (std::string(assertion_info.name) + (arg_text ? " ( " + std::string(arg_text) + " ) " : ""));
        }

        constexpr bool passed() const noexcept
        {
            return (result() == AssertionResult::SUCCESS);
        }

        constexpr bool failed() const noexcept
        {
            return (result() == AssertionResult::FAIL);
        }

    private:
        AssertionResult result_;
        TestInfo assertion_info;
        LineInfo line_info;
        StringView arg_text;

    };

    template <typename... Args>
    constexpr Assertion<Args...> make_assertion(TestInfo test_info, LineInfo line_info, StringView arg_text, comparison::CompareHelper<Args...>&& compare) noexcept
    {
        return Assertion<Args...>(test_info, line_info, arg_text, std::forward<comparison::CompareHelper<Args...>>(compare));
    }

    constexpr Assertion<bool> make_assertion(TestInfo test_info, LineInfo line_info, StringView arg_text, bool eval) noexcept
    {
        return Assertion<bool>(test_info, line_info, arg_text, eval);
    }
}

////////////////////////////////////////////////////////////////////////////////
/// BEGIN SSTEST MACROS
////////////////////////////////////////////////////////////////////////////////

#if defined(__clang__)

#   define INTERNAL_SSTEST_SUPPRESS_ASSERTION_WARNINGS_BEGIN _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Woverloaded-shift-op-parentheses\"")
#   define INTERNAL_SSTEST_SUPPRESS_ASSERTION_WARNINGS_END _Pragma("clang diagnostic pop")

#else

#   define INTERNAL_SSTEST_SUPPRESS_ASSERTION_WARNINGS_BEGIN
#   define INTERNAL_SSTEST_SUPPRESS_ASSERTION_WARNINGS_END

#endif 


#define INTERNAL_SSTEST_MESSAGE() \
        ::sstest::TestRunner::getInstance().reporter() += ::sstest::TestRunner::Reporter::StreamObject()

#define INTERNAL_SSTEST_EXIT() \
        return INTERNAL_SSTEST_MESSAGE()

#define INTERNAL_SSTEST_CONTINUE() \
        INTERNAL_SSTEST_MESSAGE()

#define INTERNAL_SSTEST_SUCCESS() \
        INTERNAL_SSTEST_EXIT()

#define INTERNAL_SSTEST_FAILURE() \
        ::sstest::TestRunner::getInstance().explicitFailure(); \
        INTERNAL_SSTEST_EXIT()

#define INTERNAL_SSTEST_CONFIGURE(...) \
        ::sstest::TestRunner::getInstance().configure(__VA_ARGS__)

#define INTERNAL_SSTEST_SHOW_PASSED_ASSERTIONS() \
        ::sstest::TestRunner::getInstance().configure() = ::sstest::TestRunner::getInstance().configure() || ::sstest::TestRunner::Configuration(true, false, true, false)
        
#define INTERNAL_SSTEST_ASSERTION(macro_name, text, on_fail, compare) \
        if (!::sstest::TestRunner::getInstance().reportAssertion( \
                ::sstest::make_assertion( ::sstest::TestInfo(macro_name), ::sstest::LineInfo(__FILE__, __LINE__), text, compare) \
                ) \
            ) \
            on_fail()

#define INTERNAL_SSTEST_ASSERTION_BOOL(macro_name, text, on_fail, eval) \
        if (!::sstest::TestRunner::getInstance().reportAssertion( \
                ::sstest::make_assertion( ::sstest::TestInfo(macro_name), ::sstest::LineInfo(__FILE__, __LINE__), text, (eval) ) \
                ) \
            ) \
            on_fail()

#define INTERNAL_SSTEST_ASSERTION_UNPACK(macro_name, text, on_fail, ...) \
        if (!::sstest::TestRunner::getInstance().reportAssertion( \
                ::sstest::make_assertion( ::sstest::TestInfo(macro_name), ::sstest::LineInfo(__FILE__, __LINE__), text, \
                INTERNAL_SSTEST_SUPPRESS_ASSERTION_WARNINGS_BEGIN \
                (::sstest::comparison::Decomposer() << __VA_ARGS__) ) \
                INTERNAL_SSTEST_SUPPRESS_ASSERTION_WARNINGS_END \
                ) \
            ) \
            on_fail()

// TODO throw assertions

#define INTERNAL_SSTEST_ASSERTION_INFER(macro_name, expr, on_fail) \
        INTERNAL_SSTEST_ASSERTION_UNPACK(macro_name, #expr, on_fail, expr)

#define INTERNAL_SSTEST_ASSERTION_NOT(macro_name, expr, on_fail) \
        INTERNAL_SSTEST_ASSERTION(macro_name, #expr, on_fail, (::sstest::comparison::make_negation_compare(expr)) )

#define INTERNAL_SSTEST_ASSERTION_BOOL_TRUE(macro_name, expr, on_fail) \
        INTERNAL_SSTEST_ASSERTION_BOOL(macro_name, #expr, on_fail, (expr) )

#define INTERNAL_SSTEST_ASSERTION_BOOL_FALSE(macro_name, expr, on_fail) \
        INTERNAL_SSTEST_ASSERTION_BOOL(macro_name, #expr, on_fail, !(expr) )

#define INTERNAL_SSTEST_ASSERTION_NULL(macro_name, expr, on_fail) \
        INTERNAL_SSTEST_ASSERTION(macro_name, #expr, on_fail, (::sstest::comparison::make_negation_compare(expr)) )

#define INTERNAL_SSTEST_ASSERTION_NOT_NULL(macro_name, expr, on_fail) \
        INTERNAL_SSTEST_ASSERTION(macro_name, #expr, on_fail, (::sstest::comparison::make_compare(expr)) )

#define INTERNAL_SSTEST_ASSERTION_PRED(macro_name, predicate, on_fail, ...) \
        INTERNAL_SSTEST_ASSERTION(macro_name, #predicate "( " #__VA_ARGS__ " )", on_fail, ::sstest::comparison::make_compare(predicate, __VA_ARGS__) )

#define INTERNAL_SSTEST_ASSERTION_PRED_HIDE(macro_name, predicate, on_fail, ...) \
        INTERNAL_SSTEST_ASSERTION(macro_name, #__VA_ARGS__, on_fail, ::sstest::comparison::make_compare(predicate, __VA_ARGS__) )

#define INTERNAL_SSTEST_ASSERTION_EQ2(macro_name, lhs, rhs, on_fail) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #lhs ", " #rhs, \
            on_fail, \
            ::sstest::comparison::make_equal_compare(lhs, rhs) \
        ) 

#define INTERNAL_SSTEST_ASSERTION_NEQ2(macro_name, lhs, rhs, on_fail) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #lhs ", " #rhs, \
            on_fail, \
            ::sstest::comparison::make_not_equal_compare(lhs, rhs) \
        ) 

#define INTERNAL_SSTEST_ASSERTION_LE(macro_name, lhs, rhs, on_fail) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #lhs ", " #rhs, \
            on_fail, \
            ::sstest::comparison::make_less_equal_compare(lhs, rhs) \
        ) 

#define INTERNAL_SSTEST_ASSERTION_GE(macro_name, lhs, rhs, on_fail) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #lhs ", " #rhs, \
            on_fail, \
            ::sstest::comparison::make_greater_equal_compare(lhs, rhs) \
        ) 

#define INTERNAL_SSTEST_ASSERTION_LESS(macro_name, lhs, rhs, on_fail) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #lhs ", " #rhs, \
            on_fail, \
            ::sstest::comparison::make_less_compare(lhs, rhs) \
        ) 

#define INTERNAL_SSTEST_ASSERTION_GREATER(macro_name, lhs, rhs, on_fail) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #lhs ", " #rhs, \
            on_fail, \
            ::sstest::comparison::make_greater_compare(lhs, rhs) \
        ) 

#define INTERNAL_SSTEST_ASSERTION_EQALL(macro_name, on_fail, ...) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #__VA_ARGS__, \
            on_fail, \
            ::sstest::comparison::make_all_equal_first_compare(__VA_ARGS__) \
        )

#define INTERNAL_SSTEST_ASSERTION_EQALLEACH(macro_name, on_fail, ...) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #__VA_ARGS__, \
            on_fail, \
            ::sstest::comparison::make_all_equal_each_compare(__VA_ARGS__) \
        )

#define INTERNAL_SSTEST_ASSERTION_ANY(macro_name, on_fail, ...) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #__VA_ARGS__, \
            on_fail, \
            ::sstest::comparison::make_any_compare(__VA_ARGS__) \
        )

#define INTERNAL_SSTEST_ASSERTION_ALL(macro_name, on_fail, ...) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #__VA_ARGS__, \
            on_fail, \
            ::sstest::comparison::make_all_compare(__VA_ARGS__) \
        )

#define INTERNAL_SSTEST_ASSERTION_ASC(macro_name, on_fail, ...) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #__VA_ARGS__, \
            on_fail, \
            ::sstest::comparison::make_ascending_compare(__VA_ARGS__) \
        )

#define INTERNAL_SSTEST_ASSERTION_DESC(macro_name, on_fail, ...) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #__VA_ARGS__, \
            on_fail, \
            ::sstest::comparison::make_descending_compare(__VA_ARGS__) \
        )

#define INTERNAL_SSTEST_ASSERTION_APPROX(macro_name, lhs, rhs, del, on_fail) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #lhs ", " #rhs ", " #del, \
            on_fail, \
            ::sstest::comparison::make_approx_equal_compare(lhs, rhs, del) \
        )

#define INTERNAL_SSTEST_ASSERTION_EQFLOAT(macro_name, lhs, rhs, on_fail) \
        INTERNAL_SSTEST_ASSERTION(macro_name, \
            #lhs ", " #rhs, \
            on_fail, \
            ::sstest::comparison::make_float_equal_compare(lhs, rhs) \
        )


#endif // _SSTEST_ASSERTION_H_
