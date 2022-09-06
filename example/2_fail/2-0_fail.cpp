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

#include <sstest/sstest_include.h>

/**
 * \file 2-0_fail.cpp
 * \brief SSTest example #1 file #1: single failing test
 * This example consists of a single file which contains a single test using the TEST() macro, which uses the FAIL() macro to explicitly fail the test 
 * Run when, the output should inform the user the total number of tests ran (1) and the total number of tests passed (0)
 */

// Using the TEST() macro, define an anonymous test
// - The test will be automatically added to a list of test that will be ran on execution
// - There can only be ONE anonymous test per executable
TEST()
{
    // FAIL() will automatically mark the test as failed
    FAIL();
}

// This is not allowed as there can only be one anonymous test
// TEST() {} 