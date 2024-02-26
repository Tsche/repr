cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(repr CONFIG REQUIRED)

add_executable(example src/example.cpp)

option(ENABLE_FMTLIB "Use fmt::* instead of std::*" OFF)
option(ENABLE_MAGIC_ENUM "Use magic_enum instead of librepr::ctei" OFF)

if(ENABLE_FMTLIB)
  message(STATUS "Using fmt")
  find_package(fmt CONFIG REQUIRED)
  target_link_libraries(example PRIVATE fmt::fmt)
  target_compile_definitions(example PRIVATE REPR_FORMAT_FMT=ON)
endif()

if (ENABLE_MAGIC_ENUM)
  message(STATUS "Using magic_enum")

  find_package(magic_enum CONFIG REQUIRED)
  target_link_libraries(example PRIVATE magic_enum::magic_enum)
  target_compile_definitions(example PRIVATE REPR_USE_MAGIC_ENUM=ON)
endif()

target_link_libraries(example PRIVATE repr::repr)