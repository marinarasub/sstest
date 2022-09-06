# Tutorial

## Writing Your First Test

**Make sure to [build](../README.md) SSTest as a static library first.**

This test is intended for beginners, so you can skip to the [reference](reference.md) if you already know the basics of unit testing.

### Part I: Let's Begin!

> *This tutorial will be done in a Linux environment, but you can follow along on Windows or another environment, although you will have to know how to build and run a C++ program in your environment.*

To begin, create an empty C++ file called `main.cpp` and include the main header file in the include directory with `#include <sstest/sstest_include.h>`. Next, put `sstest.a` in your working directory. 

Now, let's try compiling the empty file and linking sstest with your favorite C++ compiler and see what happens. I am using g++ with the comamnd `g++ -o main main.cpp -lsstest_main -lsstest`.

> *Note: order is important! Make sure you write `-lsstest_main -lsstest` and not `-lsstest -lsstest_main`*

Next, run from the shell with the command `./main`

> *Note: make sure your compiler supports C++11*

What do you see? Hopefully you'll see some text outputted to the shell that says something along the lines of `No tests to run`.

Next, let's try writing a empty test function. To do this simply write: 
```
TEST()
{

}
```
Now build once again: `g++ -o main main.cpp -lsstest_main -lsstest`.

This time you should you see 1 test ran, which passed!

### Part II: Testing a Function

Now, let's try to test some actual functionality. I'm going to write a function called `add()` which adds two integers. We are going to write some tests to make sure our `add()` function does what we want it to.

```
// add two integers and return result
int add(int a, int b)
{
    return a + b;
}
```
To make sure `add()` gives us results we expect it to, let's write a few *assertions*. Assertions are runtime checks that makes sure that a expected condition or state holds true. If an assertion does not evaluate to the expected result, the current test will fail.

Here in SSTest, we can use `REQUIRE()`, which can be used like a C++ function to write a few assertions as below:
```
TEST()
{
    // check 0 + 0  == 0
    REQUIRE(0 == add(0, 0));
    // check 1 + 0  == 1, both left and right hand side
    REQUIRE(1 == add(0, 1));
    REQUIRE(1 == add(1, 0));
    // check 1 + 1 == 2
    REQUIRE(2 == add(1, 1));
    // check 2 + 1 == 3, both left and right hand side
    REQUIRE(3 == add(2, 1));
    REQUIRE(3 == add(1, 2));
    // check 1 + -1 == 0
    REQUIRE(2 == add(1, -1));
}
```

> *Hint: There are also built-in macros for common types of comparison such as `REQUIRE_EQUAL()`. Take a look [here](reference.md).*

Once again, build: `g++ -o main main.cpp -lsstest_main -lsstest` and run.

Oops! One of the tests failed. I made a typo on the last assertion - it should be `0 == add(1, -1)` instead. Let's fix that. After fixing the typo, we should see that all tests passed!

Alright, that's enough math for the next decade or so. Let's try writing another function, which checks if a string is empty or not.

```
// return true if string is empty (first character is nul char)
bool is_string_empty(const char* str)
{
    return *str == '\0';
}
```

Oh wait a minute, that has nothing to do with addition, we should probably make a seperate test function. We can do so by specifiying the name of the test function as a parameter to `TEST()`. We should probably rename our old test for `add()` too.
```
TEST(test_add)
{
    ...
}

TEST(test_string_empty)
{
    // Now it's your turn! write some tests here...
}
```

> *Hint: Also try out the different macros from the `REQUIRE()` or even `EXPECT()` family in different orders and see what happens.*

### Part III: Test Suites
Alright, that was good fun, but what if we need to test more complex data types and/or functions? In this section we'll go over test suites and test fixtures. 

*Coming soon!*

There are many ways to write tests, but typically I like a 1:1 ratio of test suite to user function to cover N test cases (or an arbitrary number of different scenarios) per user function.

## Next Steps

Hopefully you are now comfortable enough to start expirimenting and writing your own tests! View the [reference](reference.md) for a full list of available features. If you found a bug or have any suggestions, please let us know!

**Good luck :)**
