# example dir for sstest
cmake_minimum_required(VERSION 3.1)

link_libraries(sstest_main sstest)

include_directories("${SSTEST_INC_DIR}")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(example_0_blank
	"${SSTEST_INC_DIR}/sstest/sstest_include.h"
	"0_blank/0-0_blank.cpp"
)

add_executable(example_1_pass
	"${SSTEST_INC_DIR}/sstest/sstest_include.h"
	"1_pass/1-0_pass.cpp"
)

add_executable(example_2_fail
	"${SSTEST_INC_DIR}/sstest/sstest_include.h"
	"2_fail/2-0_fail.cpp"
)

add_executable(example_3_basic
	"${SSTEST_INC_DIR}/sstest/sstest_include.h"
	"3_basic/3-0_basic.cpp"
	"3_basic/3-1_basic.cpp"
)

add_executable(example_4_user_type
	"${SSTEST_INC_DIR}/sstest/sstest_include.h"
	"example_common.h"
	"4_user_type/4-0_user_type.cpp"
)

add_executable(example_5_fixture
	"${SSTEST_INC_DIR}/sstest/sstest_include.h"
	"5_fixture/5-0_fixture.cpp"
)

add_executable(example_6_parameterized
	"${SSTEST_INC_DIR}/sstest/sstest_include.h"
	"6_parameterized/6-0_parameterized.cpp"
)

add_executable(A_tutorial
	"${SSTEST_INC_DIR}/sstest/sstest_include.h"
	"A_tutorial/A-0_tutorial.cpp"
)

set_target_properties(
	example_0_blank
	example_1_pass
	example_2_fail
	example_3_basic
	example_4_user_type
	example_5_fixture
	example_6_parameterized
	A_tutorial
	PROPERTIES FOLDER example)