project (PROJECT_NAME)
cmake_minimum_required (VERSION 2.8.9)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++14 -Werror -Wall -pedantic -Wcast-align -Wempty-body -Wformat-security -Winit-self -Warray-bounds")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

set (CMAKE_CXX_FLAGS_DEBUG "-g3 -O0")
set (CMAKE_CXX_FLAGS_RELEASE "-O2")

set (CMAKE_AUTOMOC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(src)

option(ENABLE_TESTS "Bulid tests" ON)

if (ENABLE_TESTS)
  find_package(Gtest REQUIRED)
  add_subdirectory(tests)
endif ()
