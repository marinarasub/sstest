###############################################################################
# Makefile for SSTest: A simple C++ testing library
# Copyright © 2022 David Lu
# 
# BUILDING
# If you have make and a g++ installed, it's as easy as
# >		make
# You can switch compilers (i.e. clang++) by specifying 
# >		make CXX=<c++ compiler> LD=<c++ linker>
#
# TARGETS
# To build a specific target, specify the target to the command make
# >		make <target>
# - sstest - build the core SSTest static library
# - sstest_main - build the library containing default main() function, dependent on sstest
# 	- unless you want to use a custom main() function, you'll want to build this too
# - all (default) - build all targets
# - test - build tests
# - example - build example executables
# - clean - delete build output files
#
# CONFIGURING
# To use a configuration, use an argument:
# >		make config=<config>
# *note these are case sensitive*
# - Release (Default) - build for use with your project 
# - RelWithDebInfo - build for release with debug information
# - MinSizeRel - build release with minimum code size
# - Debug - build for development
# By default, the output directory is out/$(config)
#
###############################################################################

# compile and link options
CXX = g++
CXXFLAGS = -std=c++11 -pedantic-errors -Wall -Werror -Wfatal-errors -Wextra -Wdangling-else -Wconversion
LD = g++
LDFLAGS = -std=c++11 -pedantic-errors -Wall -Werror -Wfatal-errors -Wextra -Wdangling-else -Wconversion
AR = ar

debug_flags = -Wno-unused-parameter -Wno-unused-variable -Wno-unused-const-variable -fstack-protector -fsanitize=address -fsanitize=undefined -fsanitize-address-use-after-scope -O0 -g
release_flags = -O2 -DNDEBUG
min_size_release_flags = -Os -DNDEBUG

config = Release

# add additonal compiler options per configuration 
ifeq ($(config), Debug)
CXXFLAGS += $(debug_flags)
LDFLAGS += $(debug_flags) 
endif

ifeq ($(config), Release)
CXXFLAGS += $(release_flags)
LDFLAGS += $(release_flags)
endif

ifeq ($(config), RelWithDebInfo)
CXXFLAGS += $(release_flags) -g
LDFLAGS += $(release_flags) -g
endif

ifeq ($(config), MinSizeRel)
CXXFLAGS += $(min_size_release_flags)
LDFLAGS += $(min_size_release_flags)
endif

# set up build directories
inc_dirs = include
src_dirs = src

out_dir = out/$(config)
obj_dir = $(out_dir)/obj
bin_dir = $(out_dir)/bin
lib_dir = $(out_dir)/lib

# objects
sstest_objs = sstest_string.o sstest_timer.o sstest_test.o sstest_registry.o sstest_float.o sstest_summary.o sstest_info.o sstest_exception.o sstest_registrar.o sstest_console.o sstest_assertion.o sstest_printer.o sstest_runner.o sstest_run.o 
sstest_main_objs = sstest_main.o

# libs
sstest_libs = sstest_main.a sstest.a # dependencies must be later

# exes
example_exes = 0_blank 1_pass 2_fail 3_basic 4_user_type 5_fixture 6_parameterized A_tutorial
test_exes = test_assertion  test_compare test_exception test_info test_registry test_string test_summary test_test #test_command_line_options


objs = $(sstest_objs) $(sstest_main_objs)
libs = $(sstest_libs)
exes = $(example_exes) $(test_exes)

# build targets

all : directories $(libs) $(exes) 

sstest_all : $(sstest_libs)

sstest : directories sstest.a

sstest_main : directories sstest_main.a

example : directories $(libs) $(example_exes)

test : directories sstest $(test_exes)

directories :
	@mkdir -p $(obj_dir)
	@mkdir -p $(bin_dir)
	@mkdir -p $(lib_dir)

sstest.a : $(sstest_objs)
	$(AR) rcs -o $(addprefix $(lib_dir)/, $@) $(addprefix $(obj_dir)/, $^)

sstest_main.a : $(sstest_main_objs)
	$(AR) rcs -o $(addprefix $(lib_dir)/, $@) $(addprefix $(obj_dir)/, $^)

.SECONDEXPANSION:
$(example_exes) : % : $$(wildcard example/%/*.cpp) $(addprefix $(lib_dir)/, $(libs))
	$(LD) $(LDFLAGS) -I$(inc_dirs) -Iexample -o $(addprefix $(bin_dir)/, $@) $^

$(test_exes) : % : test/%.cpp $(addprefix $(lib_dir)/, sstest.a)
	$(LD) $(LDFLAGS) -I$(inc_dirs) -Itest -o $(addprefix $(bin_dir)/, $@) $^

$(objs) : %.o : $(src_dirs)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(inc_dirs) -o $(addprefix $(obj_dir)/, $@) -c $^

clean :
	rm -f $(addprefix $(bin_dir)/, $(exes)) $(addprefix $(lib_dir)/, $(libs)) $(addprefix $(obj_dir)/, $(objs))

rebuild : clean all

.PHONY: all directories sstest_all sstest sstest_main example test clean rebuild