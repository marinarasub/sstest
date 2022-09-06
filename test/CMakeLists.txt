# test dir for sstest
cmake_minimum_required(VERSION 3.1)

include_directories("${SSTEST_INC_DIR}")

link_libraries(sstest)

# tests for sstest_exception
add_executable(test_exception
    "test_exception.cpp" 
)

# tests for sstest_info
add_executable(test_info 
    "test_info.cpp" 
)

# tests for sstest_test
add_executable(test_test
    "test_test.cpp"
)

# tests for sstest_assertion
add_executable(test_assertion
    "test_assertion.cpp"
)

add_executable(test_summary
    "test_summary.cpp"
)

add_executable(test_registry
    "test_registry.cpp"
)

add_executable(test_string
    "test_string.cpp"
)

# add_executable(test_command_line_options
#     "test_command_line_options.cpp"
# )

add_executable(test_compare
    "test_compare.cpp"
)
           
set_target_properties(
    test_exception
    test_info
    test_test
    test_compare
    test_assertion
    test_summary
    test_registry
    test_string
    #test_command_line_options
	PROPERTIES FOLDER test)
    
add_test(NAME test_exception COMMAND test_exception)
add_test(NAME test_info COMMAND test_info)   
add_test(NAME test_test COMMAND test_test)
add_test(NAME test_compare COMMAND test_compare)
add_test(NAME test_assertion COMMAND test_assertion)
add_test(NAME test_summary COMMAND test_summary)
add_test(NAME test_registry COMMAND test_registry)
add_test(NAME test_string COMMAND test_string)
# add_test(NAME test_command_line_options COMMAND test_command_line_options)