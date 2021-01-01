cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER gcc-10)
set(CMAKE_CXX_COMPILER g++-10)

project(RaytracerWeekend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
include(tinyCUnit/tinyCUnit.cmake)

add_subdirectory(src)
add_subdirectory(tests)