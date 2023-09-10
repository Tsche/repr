cmake_minimum_required(VERSION 3.15)
project(repr CXX)

add_library(repr "")

target_include_directories(repr PUBLIC 
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

set_property(TARGET repr PROPERTY CXX_STANDARD 23)

option(ENABLE_TESTS "Enable tests" OFF)
if(ENABLE_TESTS)
  message(STATUS "Building unit tests")

  enable_testing()
  add_executable(repr_test "")

  find_package(GTest REQUIRED)
  target_link_libraries(repr_test PRIVATE repr)
  target_link_libraries(repr_test PRIVATE GTest::gtest GTest::gmock)
  
  include(GoogleTest)
  gtest_discover_tests(repr_test)
  
  add_subdirectory(test)
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


add_subdirectory(src)