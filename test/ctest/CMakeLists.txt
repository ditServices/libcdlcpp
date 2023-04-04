cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)

include_directories(${cdl_SOURCE_DIR})
include_directories(${cdl_LIB_DIR})

add_executable(ctest_create_cdl ctest_create_cdl.cpp assert_macro.h)
target_link_libraries(ctest_create_cdl cdl)

add_executable(ctest_savecdl ctest_savecdl.cpp)
target_link_libraries(ctest_savecdl cdl)

add_executable(ctest_read_cdl ctest_read_cdl.cpp)
target_link_libraries(ctest_read_cdl cdl)

add_test(ctest_create_cdl_result ctest_create_cdl)
add_test(ctest_save_cdl_result ctest_savecdl)
add_test(ctest_read_cdl_result ctest_read_cdl)