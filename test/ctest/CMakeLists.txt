cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)

include_directories(${cdl_SOURCE_DIR})
include_directories(${cdl_LIB_DIR})

add_executable(ctest_create_cdl ctest_create_cdl.cpp assert_macro.h)
target_link_libraries(ctest_create_cdl cdl)
add_test(ctest_create_cdl_result ctest_create_cdl)
