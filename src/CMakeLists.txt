# src dir for sstest
cmake_minimum_required(VERSION 3.1)

include_directories("${SSTEST_INC_DIR}")

add_library(sstest STATIC
    "${SSTEST_INC_DIR}/sstest/sstest_assertion.h" 
    "${SSTEST_INC_DIR}/sstest/sstest_console.h"
    "${SSTEST_INC_DIR}/sstest/sstest_compare.h"
    "${SSTEST_INC_DIR}/sstest/sstest_def.h"
    "${SSTEST_INC_DIR}/sstest/sstest_exception.h"
    "${SSTEST_INC_DIR}/sstest/sstest_float.h"
    "${SSTEST_INC_DIR}/sstest/sstest_info.h"
    "${SSTEST_INC_DIR}/sstest/sstest_run.h"
    "${SSTEST_INC_DIR}/sstest/sstest_printer.h"
    "${SSTEST_INC_DIR}/sstest/sstest_registrar.h"
    "${SSTEST_INC_DIR}/sstest/sstest_registry.h"
    "${SSTEST_INC_DIR}/sstest/sstest_runner.h"
    "${SSTEST_INC_DIR}/sstest/sstest_string.h"
    "${SSTEST_INC_DIR}/sstest/sstest_summary.h"
    "${SSTEST_INC_DIR}/sstest/sstest_test.h"  
    "${SSTEST_INC_DIR}/sstest/sstest_timer.h"
    "${SSTEST_INC_DIR}/sstest/sstest_traits.h"
    "${SSTEST_INC_DIR}/sstest/sstest_utility.h"

    "${SSTEST_SOURCE_DIR}/sstest_assertion.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_console.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_exception.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_float.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_info.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_run.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_printer.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_registrar.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_registry.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_runner.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_string.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_summary.cpp"
    "${SSTEST_SOURCE_DIR}/sstest_test.cpp"  
    "${SSTEST_SOURCE_DIR}/sstest_timer.cpp"
)

add_library(sstest_main STATIC
    "${SSTEST_INC_DIR}/sstest/sstest_main.h"
    
    "${SSTEST_SOURCE_DIR}/sstest_main.cpp"
)
