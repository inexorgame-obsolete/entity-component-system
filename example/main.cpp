#include "inexor/ecs/thread_pool.hpp"

#include <spdlog/spdlog.h>

#include <random>

using inexor::ecs::ThreadPool;

int main() {
    spdlog::set_level(spdlog::level::trace);

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<> range(2, 8);
    ThreadPool pool;

    auto res = pool.submit([&]() {
        std::vector<std::future<void>> futures;

        for (int i = 0; i < 10; i++) {
            futures.push_back(pool.submit([&]() {
                std::this_thread::sleep_for(std::chrono::seconds(range(rng)));
                spdlog::info("Hello from thread {}", pool.get_thread_id());
            }));
        }

        for (const auto &future : futures) {
            future.wait();
        }
    });

    while (res.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {}
}
