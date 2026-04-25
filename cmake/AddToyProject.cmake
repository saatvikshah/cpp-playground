# add_toy_project(
#   NAME           <name>
#   SOURCES        <src1> [src2 ...]   # optional; header-only if omitted
#   TEST_SOURCES   <t1>   [t2   ...]   # optional
#   BENCH_SOURCES  <b1>   [b2   ...]   # optional
# )
#
# The calling CMakeLists.txt's directory is exposed publicly on the library
# target so tests and benchmarks can include the project's headers directly
# (e.g. `#include "header.hpp"`).

function(add_toy_project)
  set(options)
  set(one_value_args NAME)
  set(multi_value_args SOURCES TEST_SOURCES BENCH_SOURCES)
  cmake_parse_arguments(TOY "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(NOT TOY_NAME)
    message(FATAL_ERROR "add_toy_project: NAME is required")
  endif()

  set(lib_target "${TOY_NAME}_lib")

  if(TOY_SOURCES)
    add_library(${lib_target} STATIC ${TOY_SOURCES})
    target_include_directories(${lib_target} PUBLIC
      "${CMAKE_CURRENT_SOURCE_DIR}")
    target_compile_features(${lib_target} PUBLIC cxx_std_23)
  else()
    add_library(${lib_target} INTERFACE)
    target_include_directories(${lib_target} INTERFACE
      "${CMAKE_CURRENT_SOURCE_DIR}")
    target_compile_features(${lib_target} INTERFACE cxx_std_23)
  endif()

  if(TOY_TEST_SOURCES AND BUILD_TESTING)
    set(test_target "${TOY_NAME}_tests")
    add_executable(${test_target} ${TOY_TEST_SOURCES})
    target_link_libraries(${test_target} PRIVATE ${lib_target} GTest::gmock_main)
    gtest_discover_tests(${test_target}
      DISCOVERY_MODE PRE_TEST
      PROPERTIES LABELS "${TOY_NAME}")
  endif()

  if(TOY_BENCH_SOURCES AND CPP_PLAYGROUND_BUILD_BENCHMARKS)
    set(bench_target "${TOY_NAME}_bench")
    add_executable(${bench_target} ${TOY_BENCH_SOURCES})
    target_link_libraries(${bench_target} PRIVATE ${lib_target} benchmark::benchmark_main)
  endif()
endfunction()
