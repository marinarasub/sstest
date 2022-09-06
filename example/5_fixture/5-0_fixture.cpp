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
#include "example_common.h"

#include <cstdint>
#include <algorithm>

/**
 * \file 5-0_fixture.cpp
 * \brief Examples on how to use SSTest test fixtures
 * This example demonstrates how to use xUnit style test fixtures with SSTest.
 * By using test fixtures, you can reduce repitition in similarly structured tests, such
 * as the initial values or cleanup.
 */


using namespace example;

// To define a test fixture, you can use any C++ class you like. The fixture objects will be
// the members of the class
// - The class must be default constructible
// - Note that the members must be protected or public

class TestFixture
{
public:
	// Default constructible. If you want to perform setup, you can defined a function that the default
	// constructor calls.
	TestFixture() 
	{
		init();
	}

protected:
	// Test fixture objects go here.
	linked_list<uint32_t> list;
	const size_t initial_size = 100;

private:

	const uint32_t seed = 961393;

	void init()
	{
		for (size_t i = 0; i < initial_size; i++)
		{
			list.push_back(xorshift32(seed));
		}
	}

};

// To use the test fixture, use the same macro as used with test suites. The fixture also defines a test suite.
// - The first parameter is now the test fixture class
// - The second parameter is a name for the test case, unique within the test suite
TEST(TestFixture, a)
{
	REQUIRE(list.size() == initial_size);
	MESSAGE() << list;
}

// You can also inherit the a base class designed for use with SSTest fixtures:
class TestFixtureDerived : public ::testing::Test<> // don't forget the blank template!
{
public:

	// the SetUp() function is ran before each test
	void SetUp() override
	{
		arr = new unsigned char[1024];
	}

	// TearDown() is ran to cleanup after each test
	void TearDown() override
	{
		delete[] arr;
	}

protected:
	// test fixture objects go here
	unsigned char* arr = nullptr;
};

TEST(TestFixtureDerived, a)
{
	REQUIRE_NOT_NULL(arr);
}