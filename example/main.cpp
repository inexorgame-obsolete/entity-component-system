#include "inexor/ecs/thread_pool.hpp"

#include <iostream>

int main() {
    std::cout << "System has: " << inexor::ecs::get_num_system_threads() << " threads\n";
}
