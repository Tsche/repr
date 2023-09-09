cmake_minimum_required(VERSION 3.15)
project(repr CXX)




add_library(repr src/repr.cpp)
target_include_directories(repr PUBLIC include)



set_target_properties(repr PROPERTIES PUBLIC_HEADER "include/repr.h")
install(TARGETS repr)
