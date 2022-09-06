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

#include <cstddef>
#include <cstdio>

/**
 * \file 6-0_parameterized.cpp
 * \brief Examples on how to use SSTest parameterized tests
 * As an alternative to test fixtures, you can use value parameterized tests which can be run with a set of provided arguments.
 * This is another way to reduce repitition in test code, especially if you want to run the same test only varying the test values.
 */


// To use a test template, use the macro TEST_PARAMETERIZED_TEMPLATE()
// - The first argument is the test suite/fixture (required)
// - The second argument is the name of the test template, unique within the suite
// - The rest of the arguments should be written how you would define a function parameter list (type qualified function parameters)
TEST_PARAMETERIZED_TEMPLATE(TestTemplate, parameterized, bool b, int x, int y, double d)
{
	// In the body of the test, you can define the test how you normally would, and can reference the function parameters defined
	// above.
	// Remember that since this is a test template, you need to give it values to run with later.

	if (b)
	{
		REQUIRE_ANY(x, y);
		REQUIRE(d);
	}
	else
	{
		REQUIRE_ALL(x, y, d);
	}
}

// As mentioned earlier, to use a test template we have to use TEST_PARAMETERIZED()
// You can do this multiple times.
// - The first argument is the test suite/fixture of the test template
// - The second argument is the name of the test template
// - The third argument is the name of this instance of the test template, unique within the test template
// - The rest of the arguments are the values to run the template with
TEST_PARAMETERIZED(TestTemplate, parameterized, a, false, 1, 2, 3.0)
TEST_PARAMETERIZED(TestTemplate, parameterized, b, true, 1, 2, 3.0)
TEST_PARAMETERIZED(TestTemplate, parameterized, c, false, 1, 0, -1.0)
TEST_PARAMETERIZED(TestTemplate, parameterized, d, true, 1, 0, -1.0)

// Parmaterized tests can also be used with ::testing::Test<...> with the function arguments as the template parameters 
class TestTemplateDerived : public ::testing::Test<const char*, size_t>
{
protected:

};

TEST_PARAMETERIZED_TEMPLATE(TestTemplateDerived, print, const char* str, size_t n)
{
	for (size_t i = 0; i < n; i++)
	{
		printf("%u: %s\n", unsigned(i), str);
	}
}

TEST_PARAMETERIZED(TestTemplateDerived, print, a, "hello, test!", 3U)
TEST_PARAMETERIZED(TestTemplateDerived, print, b, "goodbye, test!", 2U)