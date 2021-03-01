# 3rd Party Libraries

## from source

### autotimer

use the latest & greatest

### doctest

use the latest & greatest

## prebuilt binary (and header only)

### tbb + parallel stl

download tbb binary for linux from:
<https://github.com/oneapi-src/oneTBB/releases/tag/v2021.1.1>

> Starting from oneTBB 2021.1.1 GitHub release TBBConfig files in the binary packages are located under <tbb-root>/lib/cmake/tbb.

download parallel stl headers from:
<https://github.com/oneapi-src/oneDPL/releases/tag/20201111>

cmake integration is similar to what is done in cxxParallel/par_stl

```cmake
set(TBB_DIR ${PROJECT_SOURCE_DIR}/3rdParty/oneapi-tbb-2021.1.1/lib/cmake/tbb)
find_package(TBB REQUIRED tbb)
add_subdirectory(${PROJECT_SOURCE_DIR}/3rdParty/oneDPL-20201111) 
```
