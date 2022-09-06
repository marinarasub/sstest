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

#ifndef _SSTEST_MAIN_H_
#define _SSTEST_MAIN_H_

/**
 * \file sstest_main.h
 * \brief Contains definition of default main function supplied by sstest_main
 * 
 */

namespace testing
{
    /**
     * \brief Implementation function to be called by default SSTest supplied main()
     * Configures command line arguments for test runner
     * By default, runs all registered tests
     * 
     * \param argc 
     * \param argv 
     * \return int 
     */
    int sstest_main(int argc, char** argv);

}

/**
 * \brief Default main function supplied by SSTest.
 * \sa sstest_main
 * 
 * \param argc Program argc
 * \param argv Program argv
 * \return Exit code
 */
int main(int argc, char** argv);

#endif // _SSTEST_MAIN_H_