###############################################################################
# A simple C++ testing library
# Copyright © 2022 David Lu
#
# Bash script for testing build and unit tests with CMake/ctest
#
###############################################################################

SCRIPT_PATH="${BASH_SOURCE:-$0}"
ROOT_DIR=$(dirname "${SCRIPT_PATH}")/../

# cd to top level dir
cd ${ROOT_DIR}

# clean if previous build
if  [ $? -eq 0 ]; then
    cmake --build ${ROOT_DIR}/build/ --target clean
else
    exit
fi

# cmake config and generate
# clang++ as well
cmake -S ${ROOT_DIR} -B ${ROOT_DIR}/build/  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ # -DCMAKE_VERBOSE_MAKEFILE=ON

# try build
if  [ $? -eq 0 ]; then
    cmake --build ${ROOT_DIR}/build/
fi

# try run tests
if  [ $? -eq 0 ]; then
    cd ${ROOT_DIR}/build/
    ctest -V
fi
