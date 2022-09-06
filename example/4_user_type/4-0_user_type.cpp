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

#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <array>
#include <initializer_list>
#include <utility>
#include <string>
#include <algorithm>

/**
 * \file 4-0_user_type.cpp
 * \brief A more practical example using custom types
 * This example consists of a multiple files which demonstrate user of SSTest with
 * user types. Tests are written to pass so you are encouraged to try changing values and/or assertion uses to
 * see what happens.
 */


using namespace example;

// Example class #1: A simple singly-linked list class (common.h)
// A basic example showing how you would use SSTest

TEST(test_4_0_linked_list, construct_int_list_empty)
{
	//SHOW_PASSED_ASSERTIONS();

	linked_list<int> list;
	REQUIRE(list.empty());
	REQUIRE(list.size() == 0U);
}

TEST(test_4_0_linked_list, add_back_int_list)
{
	//SHOW_PASSED_ASSERTIONS();

	linked_list<int> list;
	// add 1 element
	list.push_back(1);
	REQUIRE(!list.empty());
	REQUIRE(list.size() == 1U);
	REQUIRE_EQUAL(list.front(), 1);
	REQUIRE_EQUAL(list.back(), 1);

	// add 1 more element
	list.push_back(2);
	REQUIRE(!list.empty());
	REQUIRE(list.size() == 2U);
	REQUIRE_EQUAL(list.front(), 1);
	REQUIRE_EQUAL(list.back(), 2);

	// clear the list
	list.clear();
	REQUIRE(list.empty());
	REQUIRE(list.size() == 0U);
}

TEST(test_4_0_linked_list, add_front_int_list)
{
	//SHOW_PASSED_ASSERTIONS();

	linked_list<int> list;
	// add 1 element
	list.push_front(1);
	REQUIRE(!list.empty());
	REQUIRE(list.size() == 1U);
	REQUIRE_EQUAL(list.front(), 1);
	REQUIRE_EQUAL(list.back(), 1);

	// add 1 more element
	list.push_front(2);
	REQUIRE(!list.empty());
	REQUIRE(list.size() == 2U);
	REQUIRE_EQUAL(list.front(), 2);
	REQUIRE_EQUAL(list.back(), 1);

	// clear the list
	list.clear();
	REQUIRE(list.empty());
	REQUIRE(list.size() == 0U);
}

TEST(test_4_0_linked_list, init_list_int_list)
{
	//SHOW_PASSED_ASSERTIONS();

	// empty list
	linked_list<int> list = {};
	REQUIRE(list.empty());
	REQUIRE(list.size() == 0U);

	list = { -1, 17, 4, 76, -144 };
	REQUIRE(!list.empty());
	REQUIRE(list.size() == 5U);
	REQUIRE(list[0] == -1);
	REQUIRE(list[1] == 17);
	REQUIRE(list[2] == 4);
	REQUIRE(list[3] == 76);
	REQUIRE(list[4] == -144);
}

TEST(test_4_0_linked_list, map_double_int_list)
{
	//SHOW_PASSED_ASSERTIONS();

	linked_list<int> list;

	const int max = 10;
	for (int i = 0; i < max; i++)
	{
		size_t sz = static_cast<size_t>(i);
		list.push(i);
		REQUIRE_EQUAL(0, list.front());
		REQUIRE_EQUAL(i, list.back());
		REQUIRE_EQUAL(i, list[sz]);
		REQUIRE_EQUAL(sz + 1, list.size());
	}

	linked_list<int> doubled_list = list;

	doubled_list.map([](int x) -> int { return x * 2; });

	for (const int& i : list)
	{
		size_t sz = static_cast<size_t>(i);
		REQUIRE_EQUAL(i * 2, doubled_list[sz]);
	}

}

TEST(test_4_0_linked_list, insert_int_list)
{
	//SHOW_PASSED_ASSERTIONS();

	linked_list<int> list;

	// insert at position 0
	list.insert(-1, 0);
	REQUIRE(!list.empty());
	REQUIRE(list.size() == 1U);
	REQUIRE_EQUAL(-1, list[0]);

	// insert at end
	list.insert(-2, 1);
	REQUIRE(!list.empty());
	REQUIRE(list.size() == 2U);
	REQUIRE_EQUAL(-1, list[0]);
	REQUIRE_EQUAL(-2, list[1]);

	// insert in middle
	list.insert(-3, 1);
	REQUIRE(!list.empty());
	REQUIRE(list.size() == 3U);
	REQUIRE_EQUAL(-1, list[0]);
	REQUIRE_EQUAL(-3, list[1]);
	REQUIRE_EQUAL(-2, list[2]);
}

TEST(test_4_0_linked_list, reverse_int_list)
{
	//SHOW_PASSED_ASSERTIONS();

	linked_list<int> list;

	const int max = 10;
	for (int i = 0; i < max; i++)
	{
		size_t sz = static_cast<size_t>(i);
		list.push(i);
		REQUIRE_EQUAL(0, list.front());
		REQUIRE_EQUAL(i, list.back());
		REQUIRE_EQUAL(i, list[sz]);
		REQUIRE_EQUAL(sz + 1, list.size());
	}

	list.reverse();
	REQUIRE_EQUAL(list.size(), 10U);

	for (int i = 0; i < max; i++)
	{
		REQUIRE(!list.empty());
		REQUIRE_EQUAL(i, list.pop_back());
	}
}

// Example #2: The following will have examples on how to compare between custom classes (using operators)  (common.h)
// Main concepts:
// - More assertion macros, e.g. approximate, less/greater, custom predicate
// - Value printing
//		- You can define your own print function that SSTest will use in a few ways, but here we are using a to_string function (see example_common.h)

TEST(test_4_0_vector2, construct)
{
	//SHOW_PASSED_ASSERTIONS();

	vector2f vec;
	REQUIRE_EQUAL(vec, vector2f::zero());
	REQUIRE_FALSE(vec);

	vec = vector2f(1.0f, 0.0f);
	REQUIRE_EQUAL(vec.x, 1.0f);
	REQUIRE_EQUAL(vec.y, 0.0f);

	vec = vector2f(0.0f, 1.0f);
	REQUIRE_EQUAL(vec.x, 0.0f);
	REQUIRE_EQUAL(vec.y, 1.0f);

	vec = vector2f(1.0f, 1.0f);
	REQUIRE_EQUAL(vec.x, 1.0f);
	REQUIRE_EQUAL(vec.y, 1.0f);

	vec = vector2f(1.0f, -1.0f);
	REQUIRE_EQUAL(vec.x, 1.0f);
	REQUIRE_EQUAL(vec.y, -1.0f);

	vec = vector2f(-1.0f, 1.0f);
	REQUIRE_EQUAL(vec.x, -1.0f);
	REQUIRE_EQUAL(vec.y, 1.0f);

	vec = vector2f(-1.0f, -1.0f);
	REQUIRE_EQUAL(vec.x, -1.0f);
	REQUIRE_EQUAL(vec.y, -1.0f);
}

TEST(test_4_0_vector2, angle)
{
	//SHOW_PASSED_ASSERTIONS();

	EXPECT_APPROX(vector2f(1.0f, 0.0f).angle(), 0.0f, 0.000001);
	EXPECT_APPROX(vector2f(sqrtf(3.0f), 1.0f).angle(), vector2f::pi / 6, 0.000001);
	EXPECT_APPROX(vector2f(1.0f, sqrtf(3.0f)).angle(), vector2f::pi / 3, 0.000001);
	EXPECT_APPROX(vector2f(1.0f, 1).angle(), vector2f::pi / 4, 0.000001);
	EXPECT_APPROX(vector2f(0.0f, 1.0f).angle(), vector2f::pi / 2, 0.000001);
	EXPECT_APPROX(vector2f(-1.0f, 1.0f).angle(), 3 * vector2f::pi / 4, 0.000001);
	EXPECT_APPROX(vector2f(-1.0f, 0).angle(), vector2f::pi, 0.000001);
	EXPECT_APPROX(vector2f(-1.0f, -1.0f).angle(), 5 * vector2f::pi / 4, 0.000001);
	EXPECT_APPROX(vector2f(0.0f, -1.0f).angle(), 3 * vector2f::pi / 2, 0.000001);
	EXPECT_APPROX(vector2f(1.0f, -1.0f).angle(), 7 * vector2f::pi / 4, 0.000001);
}

TEST(test_4_0_vector2, unit)
{
	const float max = 10.0f;
	const float step = 0.03f;
	// first check 0 vector unit vector is 0 - no direction. Angle is ill-defined for zero vector do don't need to check
	REQUIRE_EQUAL(vector2f::zero().unit().magnitude(), 0.0f);
	for (float y = step; y < max; y += step)
	{
		for (float x = step; x < max; x += step)
		{
			vector2f vec = vector2f(x, y);
			// angle returned is double but we float precision is much worse than double, so can't use REQUIRE_FLOAT_EQUAL
			REQUIRE_APPROX(vec.angle(), vec.unit().angle(), 0.000001) << vec << ", " << vec.unit();
			REQUIRE_FLOAT_EQUAL(vec.unit().magnitude(), 1.0f) << vec.unit();
		}
	}
}

TEST(test_4_0_vector2, equal)
{
	//SHOW_PASSED_ASSERTIONS();

	REQUIRE_EQUAL(vector2f::zero(), vector2f::zero());

	REQUIRE_EQUAL(vector2f(0.0f, 1.0f), vector2f(0.0f, 1.0f));
	REQUIRE_EQUAL(vector2f(0.0f, 1.0f).magnitude(), vector2f(0.0f, 1.0f).magnitude());

	REQUIRE_NOT_EQUAL(vector2f(0.0f, 1.0f), vector2f(1.0f, 0.0f));
	REQUIRE_EQUAL(vector2f(0.0f, 1.0f).magnitude(), vector2f(1.0f, 0.0f).magnitude());

	REQUIRE_NOT_EQUAL(vector2f(1.0f, 1.0f), vector2f(-1.0f, -1.0f));
	REQUIRE_EQUAL(vector2f(1.0f, 1.0f).magnitude(), vector2f(-1.0f, -1.0f).magnitude());
}

TEST(test_4_0_vector2, magnitude)
{
	//SHOW_PASSED_ASSERTIONS();

	REQUIRE_GREATER(vector2f(1.0f, 0.0f), vector2f::zero());
	REQUIRE_GREATER(vector2f(0.0f, 1.0f), vector2f::zero());
	REQUIRE_GREATER(vector2f(1.0f, 1.0f), vector2f::zero());
	REQUIRE_GREATER(vector2f(-1.0f, -1.0f), vector2f::zero());

	vector2f vec(1, 1);
	REQUIRE_LESS(vec, 1.00001f * vec);
	REQUIRE_GREATER(vec, 0.99999f * vec);
	REQUIRE_LESS(vec, -1.00001f * vec);
	REQUIRE_GREATER(vec, -0.99999f * vec);
}

TEST(test_4_0_vector2, add_subtract)
{
	//SHOW_PASSED_ASSERTIONS();

	REQUIRE_EQUAL(vector2f::zero() + vector2f::zero(), vector2f::zero());
	REQUIRE_EQUAL(vector2f::zero() + vector2f(1.0f, 1.0f), vector2f(1.0f, 1.0f));
	REQUIRE_EQUAL(vector2f(1.0f, 0.0f) + vector2f(0.0f, 1.0f), vector2f(1.0f, 1.0f));

	REQUIRE_EQUAL(vector2f::zero() - vector2f::zero(), vector2f::zero());
	REQUIRE_EQUAL(vector2f::zero() - vector2f(1.0f, 1.0f), vector2f(-1.0f, -1.0f));
	REQUIRE_EQUAL(vector2f(1.0f, 0.0f) - vector2f(0.0f, 1.0f), vector2f(1.0f, -1.0f));
}

TEST(test_4_0_vector2, dot)
{
	//SHOW_PASSED_ASSERTIONS();

	REQUIRE_EQUAL(vector2f::zero() * vector2f(1.0f, 1.0f), 0.0f);
	REQUIRE_EQUAL(vector2f(1.0f, 1.0f) * vector2f(1.0f, 1.0f), 2.0f);
}

TEST(test_4_0_vector2, proj_rej)
{
	//SHOW_PASSED_ASSERTIONS();

	// varying both x and y for both vectors is n^4, so for sake of time save only use a few hand picked examples
	// (we can even use our linked list implementation!)
	linked_list<std::pair<vector2f, vector2f>> pairs = {
		{ vector2f(1.0f, 1.0f), vector2f(1.0f, 1.0f) },
		{ vector2f(1.0f, 0.0f), vector2f(0.0f, 1.0f) },
		{ vector2f(1.0f, 1.0f), vector2f(-1.0f, -1.0f) },
		{ vector2f(1.0f, 1.0f), vector2f(1.0f, 0.97f) },
		{ vector2f(4.0f, 1.0f), vector2f(5.0f, 0.0f) },
		{ vector2f(90.3f, -177.6f), vector2f(5.0f, -1.97f) },
		{ vector2f(0.00001f, 1000.0f), vector2f(1000.0f, 0.0f) },
	};

	for (const auto& pair : pairs)
	{
		vector2f v = pair.first;
		vector2f u = pair.second;

		REQUIRE_PREDICATE(vector2f::is_perp, v, v.perp());
		REQUIRE_APPROX(v * v.perp(), 0.0f, 0.000001f) << "v: " << v << ", v_perp: " << v.perp();
		REQUIRE_PREDICATE(vector2f::is_perp, u, u.perp());
		REQUIRE_APPROX(u * u.perp(), 0.0f, 0.000001f) << "u: " << u << "u_perp: " << u.perp();

		if (!vector2f::is_perp(v, u))
		{
			REQUIRE_PREDICATE(vector2f::is_parallel, v, v.proj(u));
			REQUIRE_PREDICATE(vector2f::is_parallel, u, u.proj(v));
		}
		if (!vector2f::is_parallel(v, u))
		{
			REQUIRE_PREDICATE(vector2f::is_perp, v, v.rej(u));
			REQUIRE_PREDICATE(vector2f::is_perp, u, u.rej(v));
		}

		EXPECT_EQUAL(v.proj(u), ((u * v) / v.magnitude_sqr()) * v) << "v: " << v << ", u: " << u;
		EXPECT_EQUAL(u.proj(v), ((v * u) / u.magnitude_sqr()) * u) << "v: " << v << ", u: " << u;
		EXPECT_EQUAL(v.rej(u), u - v.proj(u)) << "v: " << v << ", u: " << u;
		EXPECT_EQUAL(u.rej(v), v - u.proj(v)) << "v: " << v << ", u: " << u;
	}
}


// Example #3: comparison proxy
// This example shows how objects can be compared without const qualification

TEST(test_4_0_compare_proxy, count_compare)
{
	//SHOW_PASSED_ASSERTIONS();

	compare_proxy<bool> cp1(false);
	compare_proxy<bool> cp2(true);
	REQUIRE_NOT(cp1 == cp2);
	REQUIRE_EQUAL(1U, cp1.get_times_compared());
	REQUIRE_EQUAL(1U, cp2.get_times_compared());
	REQUIRE(cp1 < cp2);
	REQUIRE_EQUAL(2U, cp1.get_times_compared());
	REQUIRE_EQUAL(2U, cp2.get_times_compared());
	REQUIRE_NOT(cp2 < cp1);
	REQUIRE_EQUAL(3U, cp1.get_times_compared());
	REQUIRE_EQUAL(3U, cp2.get_times_compared());
	REQUIRE_FALSE(cp1.get_object());
	REQUIRE_TRUE(cp2.get_object());
}

TEST(test_4_0_compare_proxy, qsort_compare)
{
	std::array<compare_proxy<int>, 7> arr = { 2, 4, 6, 1, -5, 17, 4 };
	std::sort(arr.begin(), arr.end(), compare_proxy_compare<int, int>);

	size_t i = 0;
	size_t acc = 0;
	for (const compare_proxy<int>& cp : arr)
	{
		acc += cp.get_times_compared();
		MESSAGE() << "arr[" + std::to_string(i) + "] = " + std::to_string(cp.get_object()) + ", times compared : " << cp.get_times_compared();
		++i;
	}
	MESSAGE() << "Total times compared: " << acc;
}

