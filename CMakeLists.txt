cmake_minimum_required(VERSION 3.24)
project(cdl)

set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test/ctest)


