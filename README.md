# cpp-playground

A scratchpad for small C++23 toy projects (`unique_ptr`, `variant`, SPSC queue, ...). Every toy gets unit tests (GoogleTest) and micro-benchmarks (Google Benchmark).

## Requirements

- CMake ≥ 3.24
- Ninja
- A C++23 compiler (gcc-14 / clang-18 / Apple Clang 16+)

Dependencies (gtest, benchmark) are fetched automatically by CMake — no manual install.

## Quick start

```sh
make all   1_hello_world    # build + test + bench one project
make test  1_hello_world    # just its tests
make bench 1_hello_world    # just its benchmark
make test                   # every project's tests
make build BUILD_TYPE=Debug # debug build of everything
make clean 1_hello_world    # wipe one project's build artifacts
make clean                  # wipe the whole build dir
```

Omit the project name to act on every toy.

Raw cmake path if you want to skip the wrapper:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
./build/projects/1_hello_world/1_hello_world_bench
```

## Adding a new toy project

```sh
make new my_toy       # scaffolds projects/<N>_my_toy + updates root CMakeLists.txt
make all 2_my_toy     # build + test + bench the new toy
```

`make new <name>` requires snake_case (`^[a-z][a-z0-9_]*$`) and auto-picks the next numeric prefix. The generated toy has a trivial `answer()` placeholder with a passing test and a benchmark — swap it out for whatever you're actually exploring.

<details><summary>Manual fallback</summary>

1. Create `projects/<N>_<name>/` with `tests/` and `benchmarks/` subdirs; drop `<name>.hpp` and `<name>.cpp` at the project root.
2. Drop in a `CMakeLists.txt`:

   ```cmake
   add_toy_project(
     NAME <N>_<name>
     SOURCES <name>.cpp
     TEST_SOURCES tests/<name>_test.cpp
     BENCH_SOURCES benchmarks/<name>_bench.cpp
   )
   ```

   `NAME` must match the directory name so `make <verb> <N>_<name>` targets work.

3. Add `add_subdirectory(projects/<N>_<name>)` to the top-level `CMakeLists.txt`.

Header-only toys can omit `SOURCES`.

</details>

## Pre-commit hooks

```sh
pip install pre-commit
pre-commit install
```

`make format` runs all hooks across the tree.

## CI

GitHub Actions builds + tests on Ubuntu 24.04 against gcc-14 and clang-18 on every push and PR. Add `build-and-test` as a required status check to enforce tests pass before merging.
