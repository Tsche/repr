cmake_minimum_required(VERSION 3.15)
include(cmake/warnings.cmake)

project(repr CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_library(repr INTERFACE)

set_property(TARGET repr PROPERTY CXX_STANDARD 23)

target_include_directories(repr INTERFACE 
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

find_package(magic_enum CONFIG REQUIRED)
target_link_libraries(repr INTERFACE magic_enum::magic_enum)

option(ENABLE_SANITIZERS "Enable asan and ubsan" OFF)
option(ENABLE_COVERAGE "Enable coverage instrumentation" OFF)

if(NOT BUILD_TESTING STREQUAL OFF)
  message(STATUS "Building unit tests")

  enable_testing()
  add_executable(repr_test "")
  
  enable_warnings(repr_test)

  find_package(GTest REQUIRED)
  target_link_libraries(repr_test PRIVATE repr)
  target_link_libraries(repr_test PRIVATE GTest::gtest GTest::gmock)

  include(GoogleTest)
  gtest_discover_tests(repr_test)
  
  add_subdirectory(test)

  if (ENABLE_COVERAGE)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      target_compile_options(repr_test PRIVATE -g -O0 --coverage -fprofile-arcs -ftest-coverage -fprofile-abs-path)
      target_link_libraries(repr_test PRIVATE gcov)
      message(STATUS "Instrumenting for coverage")
    else()
      message(FATAL_ERROR "Currently only GCC is supported for coverage instrumentation.")
    endif()

  endif()

  if(ENABLE_SANITIZERS)
    target_compile_options(repr_test INTERFACE -fsanitize=address,undefined)
    target_link_options(repr_test INTERFACE -fsanitize=address,undefined)
  endif()
endif()

## headers
set_target_properties(repr PROPERTIES PUBLIC_HEADER "include/repr")
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
        DESTINATION include)

## binaries
install(TARGETS ${TARGET}
        ARCHIVE DESTINATION lib # Windows import libraries (.lib)
        RUNTIME DESTINATION bin # Windows DLLs
        LIBRARY DESTINATION lib) # shared libraries (ie Linux .so)
