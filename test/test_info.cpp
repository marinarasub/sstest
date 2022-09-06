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

#include "ctest_macros.h" 

#include <cstring>
#include "sstest/sstest_info.h"
#include "sstest/sstest_test.h"

/**
 * This class test SSTestInfo functionality
 */

using namespace sstest;

CTEST_DEFINE_TEST(line_info_construct)
{
    LineInfo info1("main.c", 1);
    CTEST_ASSERT(info1.file_name == "main.c");
    CTEST_ASSERT(info1.line_no == 1);

    LineInfo info2("???", 99999);
    CTEST_ASSERT(info2.file_name == "???");
    CTEST_ASSERT(info2.line_no == 99999);
}

CTEST_DEFINE_TEST(test_info_construct)
{
    TestInfo info1("test");
    CTEST_ASSERT(info1.name == "test");
}   

int main()
{
    CTEST_RUN_TEST(line_info_construct);
    CTEST_RUN_TEST(test_info_construct);

    return CTEST_SUCCESS;
}