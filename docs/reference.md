# User Reference

Under construction... 🛠️

## Introduction

Welcome to the SSTest user reference. This document will explain all the features that SSTest has to help you write tests.

## Basic Features

### Test Case Discovery 
The tests you write using TEST() and related macros will be stored in a collection.

### Test Runner
All assertions and test will be sent to the test runner, which can report each individual result, which will hopefully help you find you why a test failed.

## Basic Test Structure

To write a test, use the TEST() macro to define a test body with a series of statements, which is similar to a void function. The test will be automatically added to a collection of tests to be ran later by the test runner.

```
TEST() 
{
    // body...
}
```
> Anonymous global test.
You'll quickly find yourself in need to write more than one test, and you can do so by specifiying a unique identifier to the ```TEST()``` macro:
```
TEST(<test name>)
```

---
## Assertions

Test assertions are the core of testing with SSTest. What these do is check a condition against an expected result. If the condition is not met, the test runner. There is also two main types of assertions: those defined with `REQUIRE` which halt execution of the current test case if failed, and those defined with `EXPECT` which continue execution of the test case even if failed. All versions of assertions are available for both, so here we will only list `REQUIRE`. 

There are two main ways to write assertion expresssions:

### C/C++ Expressions

One way to write your assertions is how you would normally write them as a C/C++ expression. SSTest will do its best to collect the arguments used in any comparison.

*Note: SSTest assertions are not the same as C/C++'s assert() macro. Assertions here are specific to SSTest*

```
REQUIRE(1 + 1 == 2)
```
> *Example of using the plain `REQUIRE` macro, compatible with most C/C++ expressions. If this fails to compile, wrap your statement in an extra set of parentheses ( e.g. `EXPECT((x == y))` ) and let us know.*

### Predefined Comparison Helpers
You can also use predefined macros for common comparison expressions, listed below.


Check if an expression evaluates to true
```
REQUIRE_TRUE(x)
```

Check if an expression evaluatees to false
```
REQUIRE_FALSE(x)
```

Check if a value is equal to nullptr
```
REQUIRE_NULL(x)
```

Check if a value is not equal to nullptr
```
REQUIRE_NOT_NULL(x)
```

Check if two values are equal; equivalent to using the == operator
```
REQUIRE_EQUAL(x, y)
```

Check if two values are not equal; equivalent to using the != operator
```
REQUIRE_NOT_EQUAL(x, y)
```

Check if the left hand side argument is less than or equal to the right hand side argument; equivalent to using the <= operator
```
REQUIRE_LESS_EQUAL(x, y)
```

Check if the left hand side argument is greater or equal to the right hand side argument; equivalent to using the >= operator
```
REQUIRE_GREATER_EQUAL(x, y)
```

Check if the left hand side argument is strictly less than the right hand side argument; equivalent to using the < operator
```
REQUIRE_LESS(x, y)
```

Check if the left hand side argument is strictly greater than the right hand side argument; equivalent to using the > operator
```
REQUIRE_GREATER(x, y)
```

Check if all values are equal. All values are compared to the first argument, in O(n) comparisons. Equivalent to using == operator for each other argument as right hand side argument
```
REQUIRE_ALL_EQUAL(x, y, ...)
```

Check if all values are equal. All values are compared to each other, so requires O(n^2) comparisons. Equivalent to using == operator for each combination of two arguments
```
REQUIRE_ALL_EQUAL_EACH(x, y, ...)
```

Check if two floating points are equal, within a very small lenience
```
REQUIRE_FLOAT_EQUAL(x, y)
```

Equivalent to REQUIRE_FLOAT_EQUAL()
```
REQUIRE_DOUBLE_EQUAL(x, y)
```

Check if two values are within a given delta
```
REQUIRE_APPROX(x, y, del)
```

Check if all arguments evaluate, or are implicitly converted to true
```
REQUIRE_ALL(x, ...)
```

Check if for all arguments, there exists one that evaluates, or is implicitly converted to true.
```
REQUIRE_ANY(x, ...)
```

Check if the arguments are in ascending order, in order of the left most argument to the right most; equivalent to a using the <= operator between each adjacent pair of arguments
```
REQUIRE_ASCENDING(x, ...)
```

Check if the arguments are in descending order, in order of the left most argument to the right most; equivalent to a using the >= operator between each adjacent pair of arguments
```
REQUIRE_DESCENDING(x, ...)
```

Require that a custom condition is true, by supplying a custom predicate function (which returns a value convertible to bool) and its arguments
```
REQUIRE_PREDICATE(predicate, ...)
```

### Manually Controlling Test Results

If you want to manually fail a test, use the following:
```
FAIL()
```
Contrarily, if you want a test to pass and skip remaining execeution, use:
```
SUCCEED()
```

---
## Test Fixtures

You can define a test fixture using any C++ class. Any members you define can be using in any test defined with the custom fixture type. To do so, pass the testing fixture type as its first argument, and give the test case a unique identifier:
```
TEST(MyTestingFixture, <test name>)
```
> *Note: Also make sure the members are not private*

For extra support use the SSTest defined type made for testing fixtures:
```
class MyTestingFixture : public ::testing::Test<>
```
> *Note : Don't forget the you need a blank template argument ```<>```.*

---
## Paramaterized Tests

Also, you can specify arguments similar to a regular C/C++ function! Similar to test fixtures, use the following, and define your test body. Since you are defining a parameterized template, the test will not be automatically run unless you provide some values to run it with:

```
TEST_PARAMETERIZED_TEMPLATE(<test suite>, <template name>, <parameter definitions>...)
{
    // test body...
}
```

To instantiate a parameterized test template with values, use the following, with the values you want to use:
```
TEST_PARAMETERIZED(<test suite>, <template name>, <test name>, <values>...)
```

---
## Configuring the Test Runner
If the default settings aren't meeting your needs, the following is configurable:
- TODO *not implemented yet*

### Configuring From Command Line
*not implemented yet*

---
## Printing Values
By default, the test runner is able to print out built-in types. However, it may need some help printing out custom types. There are a few ways to supply your own print functions:

> *Note: If multiple of the following are defined, the first available below will be used.*
> *You cannot modify the object (must take by const& or by value), and the function must be in the global or ```sstest``` namespace.*

### SSTestPrint()
First SSTest will try to find a function called ```SSTestPrint``` with the signature:
```
SSTestPrint(<your type>, std::ostream&)
```
This one is the most specific and recommended if you want a print implementation that is only used for SSTest.

### Custom To String Function
If you already are using a to string function for your type, SSTest will try to use it. By default, the function name SSTest will look for is to_string(), however you can define a different one using 
``` 
#define TO_STRING_FUNCTION_NAME <custom name>
```

### Stream Operator Overload
The last way to do so is to use a stream operator overload with std::ostream.
```
std::ostream& operator<<(std::ostream&, <your type>)
```

If you don't define any of these for your custom type, the raw memory of an object will be printed.

