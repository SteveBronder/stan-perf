cmake_minimum_required(VERSION 3.20.2)
project(
    coinbase
    VERSION 0.0.1
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)
set(CMAKE_VERBOSE_MAKEFILE YES)
include(FetchContent)
# Externally provided libraries
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main)

FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG main) # need master for benchmark::benchmark

FetchContent_MakeAvailable(
        googletest
        googlebenchmark)

FetchContent_Declare(
  tbb
  GIT_REPOSITORY https://github.com/oneapi-src/oneTBB
  GIT_TAG        v2021.7.0  # adjust this to the version you need
)

FetchContent_MakeAvailable(tbb)

FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 3.4.0)
set(EIGEN_BUILD_DOC OFF)
# note: To disable eigen tests,
# you should put this code in a add_subdirectory to avoid to change
# BUILD_TESTING for your own project too since variables are directory
# scoped
set(BUILD_TESTING OFF)
set(EIGEN_BUILD_PKGCONFIG OFF)
FetchContent_MakeAvailable(Eigen)

FetchContent_Declare(
  sundials
  GIT_REPOSITORY https://github.com/LLNL/sundials
  GIT_TAG        v6.5.1
    # adjust this to the version you need
)

FetchContent_GetProperties(sundials)
if(NOT sundials_POPULATED)
  FetchContent_Populate(sundials)
  add_subdirectory(${sundials_SOURCE_DIR} ${sundials_BINARY_DIR})
endif()

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_STATIC_RUNTIME ON)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_Declare(
  boost
  URL https://boostorg.jfrog.io/artifactory/main/release/1.78.0/source/boost_1_78_0.tar.gz
)

FetchContent_GetProperties(boost)
if(NOT boost_POPULATED)
  FetchContent_Populate(boost)
  set(boost_INCLUDE_DIR ${boost_SOURCE_DIR})
endif()

FetchContent_Declare(
  stanmath
  GIT_REPOSITORY https://github.com/stan-dev/math
  GIT_TAG        develop # replace with the version you want to use
)

FetchContent_GetProperties(stanmath)
if(NOT stanmath_POPULATED)
  FetchContent_Populate(stanmath)
endif()

add_compile_options(
  -DNO_FPRINTF_OUTPUT 
  -DBOOST_DISABLE_ASSERTS 
  -DTBB_INTERFACE_NEW 
  -D_REENTRANT 
  -Wno-deprecated-declarations -O3)

include_directories(benchmarks)
add_subdirectory(benchmarks)
