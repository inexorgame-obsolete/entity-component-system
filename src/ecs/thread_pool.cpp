#include "inexor/ecs/thread_pool.hpp"

#include <spdlog/spdlog.h>

namespace {

constexpr std::size_t BACKUP_THREAD_COUNT = 8;

} // namespace

namespace inexor::ecs {

ThreadPool::ThreadPool(std::size_t thread_count /* = std::thread::hardware_concurrency() */) : m_stop_threads(false) {
    // NOLINTNEXTLINE
    if (!thread_count) {
        spdlog::warn("Number of CPU cores could not be determined!");
        thread_count = BACKUP_THREAD_COUNT;
    }

    spdlog::info("Initialising thread pool with {} threads", thread_count);

    for (std::size_t i = 0; i < thread_count; i++) {
        m_threads.emplace_back([&, i]() {
            std::unique_lock<std::mutex> lock(m_work_queue_mutex, std::defer_lock);

            while (true) {
                lock.lock();
                m_work_queue_cv.wait(lock, [&]() -> bool { return !m_work_queue.empty() || m_stop_threads; });

                if (m_stop_threads && m_work_queue.empty()) {
                    spdlog::trace("Thread {} stopping", i);
                    return;
                }

                spdlog::trace("Thread {}, getting work", i);
                auto temp_task = std::move(m_work_queue.front());
                m_work_queue.pop();
                lock.unlock();

                (*temp_task)();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    spdlog::info("Stopping thread pool and shutting down threads");

    m_stop_threads = true;
    m_work_queue_cv.notify_all();

    for (auto &thread : m_threads) {
        thread.join();
    }
}

int ThreadPool::num_pending_jobs() const {
    std::scoped_lock<std::mutex> lock(m_work_queue_mutex);
    return m_work_queue.size();
}

} // namespace inexor::ecs
