#!/usr/bin/env bash
set -euo pipefail

name="${1:?usage: add_toy_project.sh <snake_name>}"

if [[ ! "$name" =~ ^[a-z][a-z0-9_]*$ ]]; then
  echo "error: name must match ^[a-z][a-z0-9_]*$ (got '$name')" >&2
  exit 2
fi

script_dir="$(cd "$(dirname "$0")" && pwd)"
root="$(cd "$script_dir/.." && pwd)"
projects_dir="$root/projects"

shopt -s nullglob
for existing in "$projects_dir"/*_"$name"; do
  echo "error: project '$name' already exists at ${existing#$root/}" >&2
  exit 3
done

next=1
for d in "$projects_dir"/[0-9]*_*; do
  [ -d "$d" ] || continue
  base="${d##*/}"
  num="${base%%_*}"
  [[ "$num" =~ ^[0-9]+$ ]] || continue
  (( num >= next )) && next=$((num + 1))
done
shopt -u nullglob

full="${next}_${name}"
proj_dir="$projects_dir/$full"

mkdir -p "$proj_dir"/{src,tests,benchmarks,include/"$name"}

cat > "$proj_dir/CMakeLists.txt" <<EOF
add_toy_project(
  NAME ${full}
  SOURCES src/${name}.cpp
  TEST_SOURCES tests/${name}_test.cpp
  BENCH_SOURCES benchmarks/${name}_bench.cpp
)
EOF

cat > "$proj_dir/include/${name}/${name}.hpp" <<EOF
#pragma once

namespace ${name} {

int answer();

}  // namespace ${name}
EOF

cat > "$proj_dir/src/${name}.cpp" <<EOF
#include "${name}/${name}.hpp"

namespace ${name} {

int answer() { return 42; }

}  // namespace ${name}
EOF

cat > "$proj_dir/tests/${name}_test.cpp" <<EOF
#include "${name}/${name}.hpp"

#include <gtest/gtest.h>

TEST(${name}, AnswerIs42) {
  EXPECT_EQ(${name}::answer(), 42);
}
EOF

cat > "$proj_dir/benchmarks/${name}_bench.cpp" <<EOF
#include <benchmark/benchmark.h>

#include "${name}/${name}.hpp"

static void BM_Answer(benchmark::State& state) {
  for (auto _ : state) {
    auto v = ${name}::answer();
    benchmark::DoNotOptimize(v);
  }
}
BENCHMARK(BM_Answer);
EOF

cml="$root/CMakeLists.txt"
if [ -n "$(tail -c1 "$cml")" ]; then
  printf '\n' >> "$cml"
fi
printf 'add_subdirectory(projects/%s)\n' "$full" >> "$cml"

echo "created projects/${full}"
