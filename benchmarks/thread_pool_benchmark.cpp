#include "inexor/ecs/thread_pool.hpp"

#include <benchmark/benchmark.h>

// TODO(): Disable clang-tidy for file

using inexor::ecs::ThreadPool;

namespace {

void job_submission(benchmark::State &state) {
    ThreadPool pool;
    for (auto _ : state) {
        for (int i = 0; i < state.range(); i++) {
            pool.submit([]() {});
        }
    }
}

} // namespace

BENCHMARK(job_submission)->Arg(10000)->Unit(benchmark::kMillisecond);
BENCHMARK(job_submission)->Arg(100000)->Unit(benchmark::kMillisecond);
BENCHMARK(job_submission)->Arg(1000000)->Unit(benchmark::kMillisecond);
