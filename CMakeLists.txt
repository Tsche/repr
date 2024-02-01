cmake_minimum_required(VERSION 3.15)
#include(cmake/warnings.cmake)

project(repr CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_library(repr INTERFACE)

set_property(TARGET repr PROPERTY CXX_STANDARD 20)

target_include_directories(repr INTERFACE 
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)


option(BUILD_TESTING "Enable tests" ON)
option(ENABLE_SANITIZERS "Enable asan and ubsan" OFF)
option(ENABLE_COVERAGE "Enable coverage instrumentation" OFF)
option(ENABLE_FMTLIB "Use fmt::* instead of std::*" OFF)
option(ENABLE_MAGIC_ENUM "Use magic_enum instead of librepr::ctei" OFF)
option(ENABLE_BENCHMARK "Enable benchmarks" OFF)

if(ENABLE_FMTLIB)
  find_package(fmt CONFIG REQUIRED)
  target_link_libraries(repr INTERFACE fmt::fmt)
  target_compile_definitions(repr INTERFACE REPR_FORMAT_FMT=ON)
endif()



if (ENABLE_MAGIC_ENUM)
  message(STATUS "Using magic_enum")

  find_package(magic_enum CONFIG REQUIRED)
  target_link_libraries(repr INTERFACE magic_enum::magic_enum)
  target_compile_definitions(repr INTERFACE REPR_USE_MAGIC_ENUM=ON)
endif()

if (MSVC)
  # TODO see if we can work without a standards compliant preprocessor
  target_compile_options(repr INTERFACE "/Zc:preprocessor")
endif()


if(NOT BUILD_TESTING STREQUAL OFF)
  message(STATUS "Building unit tests")

  enable_testing()
  add_executable(repr_test "")
  
  #enable_warnings(repr_test)

  find_package(GTest REQUIRED)
  target_link_libraries(repr_test PRIVATE repr)
  target_link_libraries(repr_test PRIVATE GTest::gtest GTest::gtest_main GTest::gmock)

  include(GoogleTest)
  gtest_discover_tests(repr_test)
  
  add_subdirectory(test)

  if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      target_compile_options(repr_test PRIVATE -g -O0 --coverage -fprofile-arcs -ftest-coverage -fprofile-abs-path)
      target_link_libraries(repr_test PRIVATE gcov)
      message(STATUS "Instrumenting for coverage")
    else()
      message(FATAL_ERROR "Currently only GCC is supported for coverage instrumentation.")
    endif()
  endif()

  if(ENABLE_SANITIZERS AND WIN32)
    message(STATUS "Enabling sanitizers")
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      target_compile_options(repr_test INTERFACE -fsanitize=address)
      target_link_options(repr_test INTERFACE -fsanitize=address)
    endif()
  elseif(ENABLE_SANITIZERS)
    message(STATUS "Enabling sanitizers")
    target_compile_options(repr_test INTERFACE -fsanitize=address,undefined)
    target_link_options(repr_test INTERFACE -fsanitize=address,undefined)
  endif()
endif()

if (ENABLE_BENCHMARK)
  message(STATUS "Building runtime benchmarks")
  add_executable(repr_benchmark "")
  add_subdirectory(benchmark)
  find_package(benchmark REQUIRED)
  target_link_libraries(repr_benchmark PRIVATE benchmark::benchmark_main)
  target_link_libraries(repr_benchmark PRIVATE repr)
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
