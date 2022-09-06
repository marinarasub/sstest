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

#ifndef _SSTEST_REGISTRAR_H_
#define _SSTEST_REGISTRAR_H_

#include <type_traits>
#include "sstest_traits.h"
#include "sstest_config.h"

/**
 * \file sstest_registrar.h
 * \brief Definition for a helper class which can perform function at global level through constructor
 * Also contains internal macro definitions for registering test cases etc.
 * 
 */

namespace sstest
{
    class StringView;
    class TestFunction;

    /**
     * \brief Object for which the constructor registers a user defined test object to the test runner
     * By registering test through object, you can use this in a non-function scope
     * 
     */
    class TestRegistrar
    {
    public:
        /**
         * \brief Register a test object to the default (or global) test case
         * 
         */
        TestRegistrar(const TestFunction&);

        /**
         * \brief Register a test object under the test suite with name specified. If the test suite does not already exist,
         * it is created
         * 
         * \param suite_name 
         */
        TestRegistrar(const StringView& suite_name, const TestFunction&);
    };

#include "sstest_info.h"
#include "sstest_test.h"


////////////////////////////////////////////////////////////////////////////////
/// BEGIN SSTEST MACROS
////////////////////////////////////////////////////////////////////////////////

//The __COUNTER__ macro is helpful to keep object names unique on the same line no, but is not standard. For portability, define it as empty if not already defined.
#ifndef __COUNTER__
#define __COUNTER__
#endif // __COUNTER__

// warning suppression/disable for different compilers needed to compile properly
#if defined(_MSC_VER)

// note MSVC did not support standard _Pragma until latter version so use MSVC specific __pragma instead.
// C4099: 'identifier' : type name first seen using 'struct' now seen using 'class': should actually be interchangeable in declaration
#   define INTERNAL_SSTEST_SUPPRESS_WARNINGS_BEGIN __pragma( warning(push) ) __pragma( warning(disable : 4099) )
#   define INTERNAL_SSTEST_SUPPRESS_WARNINGS_END __pragma( warning(pop) )

#elif defined(__clang__)

// -Wmismatched-tags : struct 'except' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI 
#   define INTERNAL_SSTEST_SUPPRESS_WARNINGS_BEGIN _Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wmismatched-tags\"")
#   define INTERNAL_SSTEST_SUPPRESS_WARNINGS_END _Pragma("clang diagnostic pop")

// #elif defined(__GNUC__)

// gcc has no specific warnings to disable as of yet

#else

#   define INTERNAL_SSTEST_SUPPRESS_WARNINGS_BEGIN
#   define INTERNAL_SSTEST_SUPPRESS_WARNINGS_END

#endif

#define INTERNAL_SSTEST_CAT2(a, b) a ## b

#define INTERNAL_SSTEST_CAT(a, b) INTERNAL_SSTEST_CAT2(a, b)

// this isn't really required if function names are used.
#define INTERNAL_SSTEST_UNIQUE_NAME(name, line, counter) \
        INTERNAL_SSTEST_CAT(name, INTERNAL_SSTEST_CAT(INTERNAL_SSTEST_CAT(_l, line), INTERNAL_SSTEST_CAT(_c, counter)))

#define INTERNAL_SSTEST_TEST_NAME(test_class, test_function) \
		INTERNAL_SSTEST_CAT(stest__, INTERNAL_SSTEST_CAT(test_class, test_function)) \

#define INTERNAL_SSTEST_BASIC_TEST(function_name, test_id) \
        static void INTERNAL_SSTEST_TEST_NAME(_, function_name) (); \
        namespace {  \
            INTERNAL_SSTEST_SUPPRESS_WARNINGS_BEGIN \
            ::sstest::TestRegistrar INTERNAL_SSTEST_UNIQUE_NAME(function_name, __LINE__, __COUNTER__) (::sstest::TestFunction( \
                ::sstest::TestInfo(test_id), \
                ::sstest::LineInfo(__FILE__, __LINE__), \
                ::sstest::TestInterface::createTestInvoker(INTERNAL_SSTEST_TEST_NAME(_, function_name)) \
                )); \
            INTERNAL_SSTEST_SUPPRESS_WARNINGS_END \
        } \
        static void INTERNAL_SSTEST_TEST_NAME(_, function_name) ()

#define INTERNAL_SSTEST_NAMED_TEST(test_name) \
        INTERNAL_SSTEST_BASIC_TEST(test_name, #test_name)

// TOOD tempalte with fixture
#define INTERNAL_SSTEST_TEST_TEMPLATE(template_class, template_name,...) \
        INTERNAL_SSTEST_SUPPRESS_WARNINGS_BEGIN \
        struct template_class; \
        namespace { \
            template <typename T, typename X = void> \
            struct INTERNAL_SSTEST_TEST_NAME(template_class, template_name) { \
                void operator()(__VA_ARGS__); \
            }; \
            \
            template <typename T> \
            struct INTERNAL_SSTEST_TEST_NAME(template_class, template_name)<T, typename std::enable_if<::sstest::is_complete_type<T>::value && std::is_class<T>::value>::type> \
                : public T { \
                void operator()(__VA_ARGS__); \
            }; \
        } \
        INTERNAL_SSTEST_SUPPRESS_WARNINGS_END \
        template <> \
        void INTERNAL_SSTEST_TEST_NAME(template_class, template_name)<template_class> ::operator()(__VA_ARGS__)


#define INTERNAL_SSTEST_USE_TEST_TEMPLATE(template_class, template_name, test_name, ...) \
        namespace {  \
            INTERNAL_SSTEST_SUPPRESS_WARNINGS_BEGIN \
            ::sstest::TestRegistrar INTERNAL_SSTEST_UNIQUE_NAME(test_name, __LINE__, __COUNTER__) (#template_class, ::sstest::TestFunction( \
                ::sstest::TestInfo(#template_class "::" #template_name "." #test_name " ( " #__VA_ARGS__ " )"), \
                ::sstest::LineInfo(__FILE__, __LINE__), \
                ::sstest::TestInterface::createTestInvoker(INTERNAL_SSTEST_TEST_NAME(template_class, template_name)<template_class> (), __VA_ARGS__) \
                )); \
            INTERNAL_SSTEST_SUPPRESS_WARNINGS_END \
        }


#define INTERNAL_SSTEST_TEST_FIXTURE(test_class, test_function) \
        INTERNAL_SSTEST_SUPPRESS_WARNINGS_BEGIN \
        struct test_class; \
        namespace { \
            template <typename T, typename X = void> \
            struct INTERNAL_SSTEST_TEST_NAME(test_class, test_function) { \
                void operator()(); \
            }; \
            \
            template <typename T> \
            struct INTERNAL_SSTEST_TEST_NAME(test_class, test_function)<T, typename std::enable_if<::sstest::is_complete_type<T>::value && std::is_class<T>::value>::type> \
                : public T { \
                void operator()(); \
            }; \
            ::sstest::TestRegistrar INTERNAL_SSTEST_UNIQUE_NAME(test_name, __LINE__, __COUNTER__) (#test_class, ::sstest::TestFunction( \
                ::sstest::TestInfo(#test_class "::" #test_function), \
                ::sstest::LineInfo(__FILE__, __LINE__), \
                ::sstest::TestInterface::createTestInvoker(INTERNAL_SSTEST_TEST_NAME(test_class, test_function)<test_class> ()) \
                )); \
        } \
        INTERNAL_SSTEST_SUPPRESS_WARNINGS_END \
        template <> \
        void INTERNAL_SSTEST_TEST_NAME(test_class, test_function)<test_class> ::operator()()
        

#define INTERNAL_SSTEST_TEST_0() \
        INTERNAL_SSTEST_BASIC_TEST(_, "")

#define INTERNAL_SSTEST_TEST_1(test) \
        INTERNAL_SSTEST_NAMED_TEST(test)

#define INTERNAL_SSTEST_TEST_2(suite, test) \
        INTERNAL_SSTEST_TEST_FIXTURE(suite, test)

#define INTERNAL_SSTEST_TEST_TEMPLATE_VA(suite, template_name, ...) \
        INTERNAL_SSTEST_TEST_TEMPLATE(suite, template_name, __VA_ARGS__)

#define INTERNAL_SSTEST_USE_TEST_TEMPLATE_VA(suite, template_name, test, ...) \
        INTERNAL_SSTEST_USE_TEST_TEMPLATE(suite, template_name, test, __VA_ARGS__)

// impl. from https://newbedev.com/can-macros-be-overloaded-by-number-of-arguments
//#define CAT( A, B ) A ## B
#define SELECT( NAME, NUM ) INTERNAL_SSTEST_CAT( NAME ## _, NUM )
#define COMPOSE( NAME, ARGS ) NAME ARGS

#define GET_COUNT( _0, _1, _2, _3, _4, _5, _6 /* ad nauseam */, COUNT, ... ) COUNT
#define EXPAND() ,,,,,, // 6 commas (or 7 empty tokens)
// extra comma for iso compliance (... must be at least 1 arg)
#define VA_SIZE( ... ) COMPOSE( GET_COUNT, (EXPAND __VA_ARGS__ (), 0, 6, 5, 4, 3, 2, 1, ) )

#define VA_SELECT( NAME, ... ) SELECT( NAME, VA_SIZE(__VA_ARGS__) )(__VA_ARGS__)


#define INTERNAL_SSTEST_DEFINE_TEST(...) VA_SELECT( INTERNAL_SSTEST_TEST, __VA_ARGS__ )

#define INTERNAL_SSTEST_TEST_PARAMETERIZED_TEMPLATE(...) INTERNAL_SSTEST_TEST_TEMPLATE_VA( __VA_ARGS__ )

#define INTERNAL_SSTEST_TEST_PARAMETERIZED(...) INTERNAL_SSTEST_USE_TEST_TEMPLATE_VA( __VA_ARGS__ )



}

#endif // _SSTEST_REGISTRAR_H_