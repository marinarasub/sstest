###############################################################################
# CMakeLists for SSTest: A simple C++ testing library
# Copyright © 2022 David Lu
# 
# BUILDING
# First you need CMake (https://cmake.org/)
# CMake is a cross-platform build tool. Using CMake, you can generate various
# build files. 
#
# CONFIGURING
# There are only a few options:
# - BUILD_TEST - build test executables (written for use with ctest)
# - BUILD_EXAMPLE - build example executables
# - DEVELOPMENTAL - check this ON only if you are on a developmental branch
#
###############################################################################

cmake_minimum_required(VERSION 3.1)

# configure options
# option(BUILD_SHARED_LIBS OFF)
option(BUILD_TEST "build tests for use with ctest" ON)
option(BUILD_EXAMPLE "build examples" ON)
option(DEVELOPMENTAL ON)

project(sstest VERSION 0.1.0 LANGUAGES CXX)
set(VERSION ${CMAKE_PROJECT_VERSION})

set(CXX_STANDARD 11)
set(CXX_EXTENSIONS OFF)
set(CXX_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(GIT_BRANCH "")
if (DEVELOPMENTAL)
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endif()

# set compiler flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX /permissive- /Zc:preprocessor) # /D_CRT_SECURE_NO_WARNINGS) # TODO wall and warning removal?
    add_compile_options("$<$<CONFIG:DEBUG>:/Od>") # /fsanitize=address> only on vs2019
    add_compile_options("$<$<CONFIG:RELEASE>:/O2;/DNDEBUG>")
    if (DEVELOPMENTAL)
        add_compile_options(/DSSTEST_DEVELOPMENTAL)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-std=c++11 -pedantic-errors -Wall -Werror -Wfatal-errors -Wextra -Wdangling-else -Wconversion)
    # TODO find way to clear previous (default) options
    add_compile_options("$<$<CONFIG:DEBUG>:-O0;-g;-Wno-unused-parameter;-Wno-unused-variable;-Wno-unused-const-variable;-fstack-protector;-fsanitize=address;-fsanitize=undefined;-fsanitize-address-use-after-scope>")
    add_compile_options("$<$<CONFIG:RELEASE>:-O2;-DNDEBUG>")
    if (DEVELOPMENTAL)
        add_compile_options(-DSSTEST_DEVELOPMENTAL)
    endif()
endif()

#set(CMAKE_VERBOSE_MAKEFILE ON)

# set directories
set(SSTEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SSTEST_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

configure_file("${SSTEST_INC_DIR}/sstest/sstest_config.cmake.h.in" "${SSTEST_INC_DIR}/sstest/sstest_config.h")

add_subdirectory(src)

# unit tests with ctest
if (BUILD_TEST)
    enable_testing()
    add_subdirectory(test)
endif(BUILD_TEST)

# examples
if (BUILD_EXAMPLE)
    add_subdirectory(example)
endif(BUILD_EXAMPLE)
