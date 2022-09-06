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

#ifndef _SSTEST_RUN_H_
#define _SSTEST_RUN_H_

#include <cstdlib>
#include <vector>
#include <functional>
#include <unordered_set>
#include "sstest_string.h"
#include "sstest_runner.h"
#include "sstest_config.h"


/**
 * \file sstest_run.h
 * \brief Functions defined at global level for using SSTest
 * 
 */

namespace testing
{
    
#define SSTEST_SUCCESS EXIT_SUCCESS
#define SSTEST_FAILURE EXIT_FAILURE

    /**
     * \brief Get exit code from test results
     * 
     * \param totals 
     * \return int 
     */
    int ExitCode(const ::sstest::TestTotals& totals);

    /**
     * \brief Configure the test environment given sstest command line arguments
     * See implemenmtation of command line parsing for more details TODO
     * TODO !!! for now, this does nothing
     * 
     * \param argc 
     * \param argv 
     */
    void Configure(int argc, char** argv);

    /**
     * \brief Run tests with options specified in command line arguments 
     * If there are no additional arguments, is equivalent to RunAllTests()
     * \sa Configure()
     * \sa RunAllTests()
     * 
     * \param argc 
     * \param argv 
     * \return int 
     */
    int RunTests(int argc, char** argv);

    /**
     * \brief Run all the tests discovered by sstest with the default configuration
     * 
     * \return int 
     */
    int RunAllTests();

}

#endif // _SSTEST_RUN_H_