# cpp-playground

A scratchpad for small C++23 toy projects (`unique_ptr`, `variant`, SPSC queue, ...). Every toy gets unit tests (GoogleTest) and micro-benchmarks (Google Benchmark).

## Requirements

- CMake ≥ 3.24
- Ninja
- A C++23 compiler (gcc-14 / clang-18 / Apple Clang 16+)

Dependencies (gtest, benchmark) are fetched automatically by CMake — no manual install.

## Quick start

```sh
make run   PROJECT=1_hello_world    # configure + build + test one project
make bench PROJECT=1_hello_world    # run its benchmark
make test                           # run every project's tests
make build BUILD_TYPE=Debug         # debug build of everything
make clean
```

Raw cmake path if you want to skip the wrapper:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
./build/projects/1_hello_world/1_hello_world_bench
```

## Adding a new toy project

1. Create `projects/<N>_<name>/` with `include/`, `src/`, `tests/`, `benchmarks/` subdirs.
2. Drop in a `CMakeLists.txt`:

   ```cmake
   add_toy_project(
     NAME <N>_<name>
     SOURCES src/<name>.cpp
     TEST_SOURCES tests/<name>_test.cpp
     BENCH_SOURCES benchmarks/<name>_bench.cpp
   )
   ```

   `NAME` must match the directory name so `make PROJECT=<N>_<name>` targets work.

3. Add `add_subdirectory(projects/<N>_<name>)` to the top-level `CMakeLists.txt`.

Header-only toys can omit `SOURCES`.

## Pre-commit hooks

```sh
pip install pre-commit
pre-commit install
```

`make format` runs all hooks across the tree.

## CI

GitHub Actions builds + tests on Ubuntu 24.04 against gcc-14 and clang-18 on every push and PR. Add `build-and-test` as a required status check to enforce tests pass before merging.
