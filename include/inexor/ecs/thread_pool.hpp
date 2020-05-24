#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

// TODO(*): Implement work stealing thread pool

namespace inexor::ecs {

/// @brief A C++17 threadpool.
/// @note Seperate to the job system, the thread pool simply accepts functions to queue up.
class ThreadPool {
    // NOLINTNEXTLINE
    struct BaseContainer {
        virtual ~BaseContainer() = default;

        // TODO(*): Find a way to remove dynamic dispatch
        virtual void operator()() = 0;
    };

    template <typename Func>
    class Container : public BaseContainer {
        Func m_func;

    public:
        explicit Container(Func &&func) : m_func(std::forward<Func>(func)) {}

        void operator()() override {
            m_func();
        }
    };

private:
    std::vector<std::thread> m_threads;

    // TODO(*): Find a way to remove indirection
    std::queue<std::unique_ptr<BaseContainer>> m_work_queue;
    mutable std::mutex m_work_queue_mutex;

    std::atomic<bool> m_stop_threads;
    std::condition_variable m_work_queue_cv;

public:
    explicit ThreadPool(std::size_t thread_count = std::thread::hardware_concurrency());
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) noexcept = delete;
    ~ThreadPool();

    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool &operator=(ThreadPool &&) noexcept = delete;

    /// @brief Submit a function to the thread pool for execution.
    /// @note Func must be invocable with Args.
    template <typename Func, typename... Args, std::enable_if_t<std::is_invocable_v<Func, Args...>, int> = 0>
    auto submit(Func func, Args &&... args);

    /// @brief Returns the number of jobs waiting in the queue.
    int num_pending_jobs() const;
};

template <typename Func, typename... Args, std::enable_if_t<std::is_invocable_v<Func, Args...>, int>>
auto ThreadPool::submit(Func func, Args &&... args) {
    using invoke_type = std::invoke_result_t<Func, Args...>;

    // TODO(*): Move away from std::packaged_task
    std::packaged_task<invoke_type()> task_package(std::bind(func, std::forward<Args>(args)...));
    std::future<invoke_type> future = task_package.get_future();

    {
        std::scoped_lock<std::mutex> lock(m_work_queue_mutex);
        m_work_queue.push(std::make_unique<Container<decltype(task_package)>>(std::move(task_package)));
    }

    m_work_queue_cv.notify_one();

    return future;
}

} // namespace inexor::ecs
