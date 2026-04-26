#include <benchmark/benchmark.h>

#include "my_uniq_ptr.hpp"

static void BM_RawNewDelete(benchmark::State& state) {
  for (auto _ : state) {
    auto elem = new int(100);
    benchmark::DoNotOptimize(elem);
    delete elem;
  }
}

static void BM_UniqPtr(benchmark::State& state) {
  for (auto _ : state) {
    code::unique_ptr<int> elem = code::make_unique<int>(100);
    benchmark::DoNotOptimize(elem);
  }
}

BENCHMARK(BM_RawNewDelete);
BENCHMARK(BM_UniqPtr);
