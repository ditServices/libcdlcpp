cmake_minimum_required(VERSION 3.24)
project(cdl)

add_library(cdl cdl.cpp cdl.h)
target_link_libraries(cdl pugixml)