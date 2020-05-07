#include "inexor/ecs/thread_pool.hpp"

#include <spdlog/spdlog.h>

namespace {

constexpr std::size_t BACKUP_THREAD_COUNT = 8;

} // namespace

namespace inexor::ecs {

ThreadPool::ThreadPool(std::size_t thread_count) : stop_threads(false) {
    // NOLINTNEXTLINE
    if (!thread_count) {
        spdlog::warn("Number of CPU cores could not be determined!");
        thread_count = BACKUP_THREAD_COUNT;
    }

    spdlog::info("Initialising thread pool with {} threads", thread_count);

    for (std::size_t i = 0; i < thread_count; i++) {
        threads.emplace_back([&, i]() {
            std::unique_lock<std::mutex> lock(work_queue_mutex, std::defer_lock);

            while (true) {
                lock.lock();
                work_queue_cv.wait(lock, [&]() -> bool { return !work_queue.empty() || stop_threads; });

                if (stop_threads && work_queue.empty()) {
                    spdlog::trace("Thread {} stopping", i);
                    return;
                }

                spdlog::trace("Thread {}, getting work", i);
                auto temp_task = std::move(work_queue.front());
                work_queue.pop();
                lock.unlock();

                (*temp_task)();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    spdlog::info("Stopping thread pool and shutting down threads");

    stop_threads = true;
    work_queue_cv.notify_all();

    for (auto &thread : threads) {
        thread.join();
    }
}

int ThreadPool::get_num_waiting_jobs() const {
    std::scoped_lock<std::mutex> lock(work_queue_mutex);
    return work_queue.size();
}

} // namespace inexor::ecs
