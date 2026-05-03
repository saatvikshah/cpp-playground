cmake_minimum_required(VERSION 3.24)

project(cpp_playground LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

option(CPP_PLAYGROUND_BUILD_BENCHMARKS "Build google-benchmark targets" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(FetchContent)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.15.2
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(googletest)

if(CPP_PLAYGROUND_BUILD_BENCHMARKS)
  set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
  set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "" FORCE)
  FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        v1.9.0
    GIT_SHALLOW    TRUE
  )
  FetchContent_MakeAvailable(benchmark)
endif()

include(CTest)
include(GoogleTest)
include(AddToyProject)

add_subdirectory(projects/1_hello_world)
add_subdirectory(projects/2_my_uniq_ptr)
add_subdirectory(projects/3_event_loop)
