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

#ifndef _CTEST_MACROS_H_
#define _CTEST_MACROS_H_

#include <cstddef>
#include <cstdlib>
#include <cstdio>

/**
 * Provide minimum unit testing functionality with ctest
 */

#define CTEST_SUCCESS 0 // ctest requries return 0 as success
#define CTEST_FAILURE 1

inline static size_t counter()
{
    static size_t i = 0;
    return i++; 
}

#define CTEST_EXIT(code) \
        exit(code)

#define CTEST_DEFINE_TEST(name) \
        static void name()

#define CTEST_END_TEST() \
        return

#define CTEST_FAIL(msg) \
        printf("\tFAIL() at line %d, file %s: %s\n", __LINE__, __FILE__, msg); \
        CTEST_EXIT ( CTEST_FAILURE )

#define CTEST_ASSERT(x) \
        do { \
            if (!(x)) { \
                printf("\tAssertion failed at line %d, file %s\n", __LINE__, __FILE__); \
                CTEST_EXIT ( CTEST_FAILURE ); \
            } \
        } while (0)

#define CTEST_PRINTF(fmt, ...) \
        printf(fmt, __VA_ARGS__);

#define CTEST_RUN_TEST(name) \
        printf("RUN #%zu : %s :\n", counter() + 1, #name); \
        name (); \
        printf("... OK\n")

#endif //_CTEST_MACROS_H_