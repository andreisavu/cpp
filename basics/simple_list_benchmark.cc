#include <benchmark/benchmark.h>
#include "simple_list.h"

static void BM_SimpleListPushFront(benchmark::State& state) {
    for (auto _ : state) {
        SimpleList<int> list;
        for (int i = 0; i < state.range(0); ++i) {
            list.push_front(i);
        }
    }
}
BENCHMARK(BM_SimpleListPushFront)->Range(8, 8<<10);

static void BM_SimpleListPopFront(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        SimpleList<int> list;
        for (int i = 0; i < state.range(0); ++i) {
            list.push_front(i);
        }
        state.ResumeTiming();
        
        while (!list.empty()) {
            list.pop_front();
        }
    }
}
BENCHMARK(BM_SimpleListPopFront)->Range(8, 8<<10);

static void BM_SimpleListTransform(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        SimpleList<int> list;
        for (int i = 0; i < state.range(0); ++i) {
            list.push_front(i);
        }
        state.ResumeTiming();
        
        list.transform([](int x) { return x * 2; });
    }
}
BENCHMARK(BM_SimpleListTransform)->Range(8, 8<<10);

static void BM_SimpleListFilter(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        SimpleList<int> list;
        for (int i = 0; i < state.range(0); ++i) {
            list.push_front(i);
        }
        state.ResumeTiming();
        
        list.filter([](int x) { return x % 2 == 0; });
    }
}
BENCHMARK(BM_SimpleListFilter)->Range(8, 8<<10);

BENCHMARK_MAIN();
