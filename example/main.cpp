#include "inexor/ecs/thread_pool.hpp"

#include <spdlog/spdlog.h>

#include <chrono>
#include <vector>

using inexor::ecs::ThreadPool;

namespace {

constexpr int NUM_RUNS = 5;
constexpr int NUM_JOBS = 1000000;

} // namespace

int main() {
    spdlog::set_level(spdlog::level::info);

    ThreadPool pool;
    std::vector<std::chrono::milliseconds::rep> results;

    spdlog::info("Thread pool overhead test");
    spdlog::info("Peforming {} runs of submitting {} jobs", NUM_RUNS, NUM_JOBS);

    for (int i = 0; i < NUM_RUNS; i++) {
        auto start = std::chrono::system_clock::now();
        for (int j = 0; j < NUM_JOBS; j++) {
            pool.submit([]() {});
        }
        auto end = std::chrono::system_clock::now();

        results.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
    }

    decltype(results)::value_type avg = 0;
    for (int i = 0; i < results.size(); i++) {
        spdlog::info("{} - {}", i, results[i]);
        avg += results[i];
    }

    spdlog::info("Avg - {}", avg / results.size());
}
