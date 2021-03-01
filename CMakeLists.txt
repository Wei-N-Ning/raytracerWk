cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_COMPILER gcc-10)
set(CMAKE_CXX_COMPILER g++-10)

project(RaytracerWeekend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
#include <utility>
int main() {
    auto &[a, b] = std::pair<int, int>{1, 2};
    return 0;
}
" result)
if(result)
    message(FATAL "fail to compile c++17!!")
endif()

set(TBB_DIR ${PROJECT_SOURCE_DIR}/3rdParty/oneapi-tbb-2021.1.1/lib/cmake/tbb)
find_package(TBB REQUIRED tbb)
# Parallel STL
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdParty/oneDPL-20201111)

add_subdirectory(3rdParty/doctest)
enable_testing()
include(tinyCUnit/tinyCUnit.cmake)

add_subdirectory(src)
add_subdirectory(tests)
