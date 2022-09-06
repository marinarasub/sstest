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

#include "sstest/sstest_assertion.h"

using namespace sstest;


CTEST_DEFINE_TEST(construct_fail)
{
    Assertion<bool> assertion(TestInfo("FAIL"), LineInfo(__FILE__, __LINE__), "false", false);
    CTEST_ASSERT(assertion.result() == AssertionResult::FAIL);
    // TODO test info...
    CTEST_ASSERT(assertion.text() == "FAIL ( false ) "); // todo cosider ignore whitespace for test
}

CTEST_DEFINE_TEST(construct_pass)
{
    Assertion<bool> assertion(TestInfo("PASS"), LineInfo(__FILE__, __LINE__), "true", true);
    CTEST_ASSERT(assertion.result() == AssertionResult::PASS);
    // TODO test info...
    CTEST_ASSERT(assertion.text() == "PASS ( true ) ");
}

int main()
{
    CTEST_RUN_TEST(construct_fail);
    CTEST_RUN_TEST(construct_pass);

    return CTEST_SUCCESS;
}