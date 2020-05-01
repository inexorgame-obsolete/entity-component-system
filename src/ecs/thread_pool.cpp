#include "inexor/ecs/thread_pool.hpp"

#include <thread>

namespace inexor::ecs {

int get_num_system_threads() {
    return std::thread::hardware_concurrency();
}

} // namespace inexor::ecs
