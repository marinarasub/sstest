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

#include "sstest/sstest_compare.h"

#include "ctest_macros.h"
#include <type_traits>
#include <tuple>

using namespace sstest;
using namespace sstest::comparison;

namespace
{
    // Helpers to check type with much more forgiving syntax than type traits

    enum ref_type 
    {
        INVALID,
        RVALUE_REF,
        LVALUE_REF,
        CONST_REF,
    };

    template <typename T>
    constexpr ref_type get_ref_type_(T&&)
    {
        return RVALUE_REF;
    }

    template <typename T>
    constexpr ref_type get_ref_type_(T&)
    {
        return LVALUE_REF;
    }

    template <typename T>
    constexpr ref_type get_ref_type_(const T&)
    {
        return CONST_REF;
    }

#   define get_ref_type(x) get_ref_type_(std::forward<decltype(x)>(x))

    struct ConvertibleBool
    {
        operator bool() { return true; }
    };

    struct NonDefaultConstructible
    {
        NonDefaultConstructible() = delete;
    };

    struct NonCopyable : ConvertibleBool
    {
        NonCopyable() {}
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable(NonCopyable&&) = delete;
    };

    struct MemberCompareType : ConvertibleBool
    {
        MemberCompareType& operator==(const MemberCompareType&) { type = CONST_REF; return *this; }
        MemberCompareType& operator==(MemberCompareType&) { type = LVALUE_REF; return *this; }
        MemberCompareType& operator==(MemberCompareType&&) { type = RVALUE_REF; return *this; }
        
        ref_type type = INVALID;
    };

    struct MemberCompare : ConvertibleBool
    {
        MemberCompare operator==(const MemberCompare&) { return MemberCompare(); }
        MemberCompare operator==(MemberCompare&) { return MemberCompare(); }
        MemberCompare operator==(MemberCompare&&) { return MemberCompare(); }
    };

    struct NonCopyableResultCompare
    {
        template <typename T>
        NonCopyable& operator==(T&&) { return nc; }

        NonCopyable nc;
    };

}

CTEST_DEFINE_TEST(test_identity_primitive)
{
    int i = -1;
    const int ci = -2;

    CTEST_ASSERT(get_ref_type( identity(i) ) == LVALUE_REF);
    CTEST_ASSERT(get_ref_type( identity(ci) ) == CONST_REF);
    CTEST_ASSERT(get_ref_type( identity(int(-3)) ) == RVALUE_REF);

    CTEST_ASSERT(&identity(i) == &i);
    CTEST_ASSERT(&identity(ci) == &ci);

    CTEST_ASSERT(identity(i) == -1);
    CTEST_ASSERT(identity(ci) == -2);
    CTEST_ASSERT(identity(int(-3)) == -3);
}

CTEST_DEFINE_TEST(test_identity_noncopyable)
{
    NonCopyable o;
    const NonCopyable co;

    CTEST_ASSERT(get_ref_type( identity(o) ) == LVALUE_REF);
    CTEST_ASSERT(get_ref_type( identity(co) ) == CONST_REF);
    CTEST_ASSERT(get_ref_type( identity(NonCopyable()) ) == RVALUE_REF);

    CTEST_ASSERT(&identity(o) == &o);
    CTEST_ASSERT(&identity(co) == &co);
}

CTEST_DEFINE_TEST(test_equal)
{
    // test built in operators
    CTEST_ASSERT((1 == 2) == equal(1, 2));
    CTEST_ASSERT((1 == 1) == equal(1, 1));
    CTEST_ASSERT(get_ref_type(1 == 2) == get_ref_type(equal(1, 2)));
    CTEST_ASSERT(get_ref_type(1 == 1) == get_ref_type(equal(1, 1)));

    // test custom operators
    MemberCompareType mc;
    const MemberCompareType cmc;
    // check base logic is correct
    CTEST_ASSERT(&(mc == mc) == &mc);
    CTEST_ASSERT(&(mc == cmc) == &mc);
    CTEST_ASSERT(&(mc == MemberCompareType()) == &mc);
    CTEST_ASSERT((mc == mc).type == LVALUE_REF);
    CTEST_ASSERT((mc == cmc).type == CONST_REF);
    CTEST_ASSERT((mc == MemberCompareType()).type == RVALUE_REF);
    CTEST_ASSERT((MemberCompareType() == mc).type == LVALUE_REF);
    // argument type is deduced correctly with equal()
    CTEST_ASSERT(equal(mc, mc).type == LVALUE_REF);
    CTEST_ASSERT(equal(mc, cmc).type == CONST_REF);
    CTEST_ASSERT(equal(mc, MemberCompareType()).type == RVALUE_REF);
    CTEST_ASSERT(equal(MemberCompareType(), mc).type == LVALUE_REF);
    // return values are same value
    CTEST_ASSERT((mc == mc) == (equal(mc, mc)));
    CTEST_ASSERT((mc == cmc) == (equal(mc, cmc)));
    CTEST_ASSERT((mc == MemberCompareType()) == (equal(mc, MemberCompareType())));
    CTEST_ASSERT((mc == cmc) == (equal(equal(mc, cmc), equal(MemberCompareType(), mc))));
    // return values are same type
    CTEST_ASSERT(get_ref_type(mc == mc) == get_ref_type(equal(mc, mc)));
    CTEST_ASSERT(get_ref_type(mc == cmc) == get_ref_type(equal(mc, cmc)));
    CTEST_ASSERT(get_ref_type(mc == MemberCompareType()) == get_ref_type(equal(mc, MemberCompareType())));
    CTEST_ASSERT(get_ref_type(mc == cmc) == get_ref_type(equal(equal(mc, cmc), equal(MemberCompareType(), mc))));

    NonCopyableResultCompare ncrc;
    CTEST_ASSERT(get_ref_type(ncrc == 0) == get_ref_type(equal(ncrc, 0)));

}

CTEST_DEFINE_TEST(test_compare)
{
    CTEST_ASSERT(make_equal_compare(0, 0));
    CTEST_ASSERT(!make_equal_compare(0, -1));
}

CTEST_DEFINE_TEST(test_decomposer)
{
#if defined(__GNUC__)
    _Pragma("GCC diagnostic push")
    _Pragma("GCC diagnostic ignored \"-Wparentheses\"")
    _Pragma("GCC diagnostic ignored \"-Waddress\"")
#elif defined(__clang__)
    _Pragma("clang diagnostic push")
    _Pragma("clang diagnostic ignored \"-Wparentheses\"")
    _Pragma("clang diagnostic ignored \"-Waddress\"")
#endif
    
    // Note: CompareHelpers cannot be used as follows,
    // auto&& compare_object = (d << 1 == 2); 
    // std::get<0>(compare_object.args) == 1; // dangling reference: temporaries will only live until end of full expression
     
    Decomposer d;

    // test basic example with 1 == 1
#   define compare0 (d << 1 == 1)
    CTEST_ASSERT(compare0.result == (1 == 1));
    CTEST_ASSERT(get_ref_type(compare0.result) == get_ref_type(1 == 1));
    static_assert(std::tuple_size<decltype(compare0.args)>::value == 2, "incorrect tuple size for Collector");
    CTEST_ASSERT(std::get<0>(compare0.args) == 1);
    CTEST_ASSERT(std::get<1>(compare0.args) == 1);

    // test basic example with 1 == 2
#   define compare1 (d << 1 == 2)
    CTEST_ASSERT(compare1.result == (1 == 2));
    CTEST_ASSERT(get_ref_type(compare1.result) == get_ref_type(1 == 2));
    static_assert(std::tuple_size<decltype(compare1.args)>::value == 2, "incorrect tuple size for Collector");
    CTEST_ASSERT(std::get<0>(compare1.args) == 1);
    CTEST_ASSERT(std::get<1>(compare1.args) == 2);

    // test basic example with -1 != -2
#   define compare1b (d << -1 != -2)
    CTEST_ASSERT(compare1b.result == (-1 != -2));
    CTEST_ASSERT(get_ref_type(compare1b.result) == get_ref_type(-1 != -2));
    static_assert(std::tuple_size<decltype(compare1b.args)>::value == 2, "incorrect tuple size for Collector");
    CTEST_ASSERT(std::get<0>(compare1b.args) == -1);
    CTEST_ASSERT(std::get<1>(compare1b.args) == -2);

    // test example with more than 2 binary operators (purposely ambiguous parentheses)
#   define compare1c (d << 0x0f ^ 0x07 | 0x010)
    CTEST_ASSERT(compare1c.result == (0x0f ^ 0x07 | 0x010));
    CTEST_ASSERT(get_ref_type(compare1c.result) == get_ref_type(0x0f ^ 0x07 | 0x010));
    static_assert(std::tuple_size<decltype(compare1c.args)>::value == 3, "incorrect tuple size for Collector");
    CTEST_ASSERT(std::get<0>(compare1c.args) == 0x0f);
    CTEST_ASSERT(std::get<1>(compare1c.args) == 0x07);
    CTEST_ASSERT(std::get<2>(compare1c.args) == 0x010);

    // test example with unary operator as well
#   define compare2 (d << 0 == ~0)
    CTEST_ASSERT(compare2.result == (0 == ~0));
    CTEST_ASSERT(get_ref_type(compare2.result) == get_ref_type(0 == ~0));
    static_assert(std::tuple_size<decltype(compare2.args)>::value == 2, "incorrect tuple size for Collector");
    CTEST_ASSERT(std::get<0>(compare2.args) == 0);
    CTEST_ASSERT(std::get<1>(compare2.args) == ~0);

    // test example with non copyable type (to make sure copy is not needed to compare)
#   define compare3 (d << (NonCopyableResultCompare() == nullptr))
    CTEST_ASSERT(compare3.result == (NonCopyableResultCompare() == nullptr));
    CTEST_ASSERT(get_ref_type(compare3.result) == get_ref_type(NonCopyableResultCompare() == nullptr));
    static_assert(std::tuple_size<decltype(compare3.args)>::value == 1, "incorrect tuple size for Collector");
    CTEST_ASSERT(std::get<0>(compare3.args) == NonCopyable());

    // test example with custom comapre return type
#   define compare4 (d << MemberCompare() == MemberCompare() == MemberCompare())
    CTEST_ASSERT(compare4.result == (MemberCompare() == MemberCompare() == MemberCompare()));
    CTEST_ASSERT(get_ref_type(compare4.result) == get_ref_type(MemberCompare() == MemberCompare() == MemberCompare()));
    static_assert(std::tuple_size<decltype(compare4.args)>::value == 3, "incorrect tuple size for Collector");

    // test example with single argument (will be automatic bool)
#   define compare5 (d << 1L)
    get_ref_type(compare5.result);
    CTEST_ASSERT(compare5.result == 1L);
    CTEST_ASSERT(get_ref_type(compare5.result) == get_ref_type(1L));
    static_assert(std::tuple_size<decltype(compare5.args)>::value == 1, "incorrect tuple size for Collector");
    CTEST_ASSERT(std::get<0>(compare5.args) == 1L);

    // test example with parentheses (will be automatic bool)
#   define compare5b (d << (&d == nullptr))
    CTEST_ASSERT(compare5b.result == (&d == nullptr));
    CTEST_ASSERT(get_ref_type(compare5b.result) == get_ref_type(&d == nullptr));
    static_assert(std::tuple_size<decltype(compare5b.args)>::value == 1, "incorrect tuple size for Collector");
    CTEST_ASSERT(std::get<0>(compare5b.args) == (&d == nullptr));

    // test example with negated compare
// #   define compare6 (!(d << &d == nullptr))
//     CTEST_ASSERT(compare6.result == !(&d == nullptr));
//     CTEST_ASSERT(get_ref_type(compare6.result) == get_ref_type(!(&d == nullptr)));
//     static_assert(std::tuple_size<decltype(compare6.args)>::value == 2, "incorrect tuple size for Collector");
//     CTEST_ASSERT(std::get<0>(compare6.args) == &d);
//     CTEST_ASSERT(std::get<1>(compare6.args) == nullptr);

    // test example with lvalue argument
    unsigned ui = 3;
#   define compare7 (d << ui || (ui == 1))
    CTEST_ASSERT(compare7.result == (ui || (ui == 1)));
    CTEST_ASSERT(get_ref_type(compare7.result) == get_ref_type(ui || (ui == 1)));
    static_assert(std::tuple_size<decltype(compare7.args)>::value == 2, "incorrect tuple size for Collector");
    CTEST_ASSERT(std::get<0>(compare7.args) == ui);
    CTEST_ASSERT(std::get<1>(compare7.args) == (ui == 1));

    // test example with const argument
    const char c = '\n';
#   define compare8 (d << c >= '\0' || c == -1)
    CTEST_ASSERT(compare8.result == (c >= '\0' || c == -1));
    CTEST_ASSERT(get_ref_type(compare8.result) == get_ref_type(c >= '\0' || c == -1));
    static_assert(std::tuple_size<decltype(compare8.args)>::value == 3, "incorrect tuple size for Collector");
    CTEST_ASSERT(std::get<0>(compare8.args) == c);
    CTEST_ASSERT(std::get<1>(compare8.args) == '\0');
    CTEST_ASSERT(std::get<1>(compare8.args) == (c == -1));

#   undef compare1
#   undef compare1b
#   undef compare1c
#   undef compare2
#   undef compare3
#   undef compare4
#   undef compare5
#   undef compare5b
#   undef compare6
#   undef compare7
#   undef compare8

#if defined(__GNUC__)
    _Pragma("GCC diagnostic pop")
#elif defined(__clang__)
    _Pragma("clang diagnostic pop")
#endif
}

CTEST_DEFINE_TEST(test_all_equal_first_two)
{
    CTEST_ASSERT(all_equal_first(0, 0));
    CTEST_ASSERT(!all_equal_first(0, 1));
}

CTEST_DEFINE_TEST(test_all_equal_first_n)
{
    CTEST_ASSERT(all_equal_first(0, 0, 0));
    CTEST_ASSERT(all_equal_first(-1, -1, -1, -1));
    CTEST_ASSERT(!all_equal_first(0, 1, 2, 3, 4, 5));
    CTEST_ASSERT(!all_equal_first(3, 3, 3, 3, 4));
    CTEST_ASSERT(!all_equal_first(3, 3, 4, 3, 3));
}

CTEST_DEFINE_TEST(test_all_equal_each_two)
{
    CTEST_ASSERT(all_equal_each(0, 0));
    CTEST_ASSERT(!all_equal_each(0, 1));
}

CTEST_DEFINE_TEST(test_all_equal_each_n)
{
    CTEST_ASSERT(all_equal_each(0, 0, 0));
    CTEST_ASSERT(all_equal_each(-1, -1, -1, -1));
    CTEST_ASSERT(!all_equal_each(0, 1, 2, 3, 4, 5));
    CTEST_ASSERT(!all_equal_each(3, 3, 3, 3, 4));
    CTEST_ASSERT(!all_equal_each(3, 3, 4, 3, 3));
}

CTEST_DEFINE_TEST(test_asc_two)
{
    CTEST_ASSERT(ascending(0, 0));
    CTEST_ASSERT(ascending(0, 1));
    CTEST_ASSERT(!ascending(0, -1));
}

CTEST_DEFINE_TEST(test_asc_n)
{
    CTEST_ASSERT(ascending(0, 0, 0));
    CTEST_ASSERT(ascending(-1, -1, -1, -1));
    CTEST_ASSERT(ascending(0, 1, 2, 3, 4, 5));
    CTEST_ASSERT(ascending(3, 3, 3, 3, 4));
    CTEST_ASSERT(!ascending(4, 3, 3, 3, 3));
    CTEST_ASSERT(!ascending(3, 3, 3, 4, 3));
}

CTEST_DEFINE_TEST(test_strict_asc_two)
{
    CTEST_ASSERT(!strictly_ascending(0, 0));
    CTEST_ASSERT(strictly_ascending(0, 1));
    CTEST_ASSERT(!strictly_ascending(0, -1));
}

CTEST_DEFINE_TEST(test_strict_asc_n)
{
    CTEST_ASSERT(!strictly_ascending(0, 0, 0));
    CTEST_ASSERT(!strictly_ascending(-1, -1, -1, -1));
    CTEST_ASSERT(strictly_ascending(0, 1, 2, 3, 4, 5));
    CTEST_ASSERT(!strictly_ascending(3, 3, 3, 3, 4));
    CTEST_ASSERT(!strictly_ascending(4, 3, 3, 3, 3));
    CTEST_ASSERT(!strictly_ascending(3, 3, 3, 4, 3));
}

CTEST_DEFINE_TEST(test_all_one)
{
    CTEST_ASSERT(!all(false));
    CTEST_ASSERT(all(true));

    CTEST_ASSERT(!all(nullptr));
    CTEST_ASSERT(!all(0));
    CTEST_ASSERT(all(1));
    CTEST_ASSERT(all(&(const int&)2));
}

CTEST_DEFINE_TEST(test_all_n)
{
    CTEST_ASSERT(!all(false, true));
    CTEST_ASSERT(!all(true, false));
    CTEST_ASSERT(!all(false, false, true));
    CTEST_ASSERT(!all(true, true, false));
    CTEST_ASSERT(!all(true, false, true));

    CTEST_ASSERT(all(true, true));
    CTEST_ASSERT(all(true, true, true));
}

CTEST_DEFINE_TEST(test_any_one)
{
    CTEST_ASSERT(!all(false, false));
    CTEST_ASSERT(!all(false, false, false));

    CTEST_ASSERT(!any(false));
    CTEST_ASSERT(any(true));

    CTEST_ASSERT(!any(nullptr));
    CTEST_ASSERT(!any(0));
    CTEST_ASSERT(any(1));
    CTEST_ASSERT(any(&(const int&)2));
}

CTEST_DEFINE_TEST(test_any_n)
{
    CTEST_ASSERT(!any(false, false));
    CTEST_ASSERT(!any(false, false, false));

    CTEST_ASSERT(any(false, true));
    CTEST_ASSERT(any(true, false));
    CTEST_ASSERT(any(false, false, true));
    CTEST_ASSERT(any(true, true, false));
    CTEST_ASSERT(any(true, false, true));

    CTEST_ASSERT(any(true, true));
    CTEST_ASSERT(any(true, true, true));
}

int main()
{
    CTEST_RUN_TEST(test_identity_primitive);
    CTEST_RUN_TEST(test_identity_noncopyable);

    CTEST_RUN_TEST(test_equal);

    CTEST_RUN_TEST(test_compare);
    CTEST_RUN_TEST(test_decomposer);

    CTEST_RUN_TEST(test_all_equal_first_two);
    CTEST_RUN_TEST(test_all_equal_first_n);

    CTEST_RUN_TEST(test_all_equal_each_two);
    CTEST_RUN_TEST(test_all_equal_each_n);

    CTEST_RUN_TEST(test_asc_two);
    CTEST_RUN_TEST(test_asc_n);

    CTEST_RUN_TEST(test_strict_asc_two);
    CTEST_RUN_TEST(test_strict_asc_n);

    CTEST_RUN_TEST(test_all_one);
    CTEST_RUN_TEST(test_all_n);

    CTEST_RUN_TEST(test_any_one);
    CTEST_RUN_TEST(test_any_n);

    return CTEST_SUCCESS;
}