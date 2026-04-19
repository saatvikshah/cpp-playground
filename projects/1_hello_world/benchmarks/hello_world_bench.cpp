#include "hello_world/hello_world.hpp"

#include <benchmark/benchmark.h>

static void BM_Greet(benchmark::State& state) {
  for (auto _ : state) {
    auto s = hello_world::greet("world");
    benchmark::DoNotOptimize(s);
  }
}
BENCHMARK(BM_Greet);
