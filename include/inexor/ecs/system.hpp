#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace inexor::ecs {

class SystemManager;

struct BaseSystem {
    using family_type = std::size_t;

public:
    static family_type s_family_counter;

    BaseSystem() = default;
    BaseSystem(const BaseSystem &) = delete;
    virtual ~BaseSystem() = default;

    virtual void update(float delta_time) = 0;
};

/// @brief Base updateable system class
/// @note CRTP to avoid RTTI
template <typename>
class System : public BaseSystem {
    friend SystemManager;

public:
    static family_type family() {
        static family_type family = s_family_counter++;
        return family;
    }

    ~System() override = default;
};

class SystemManager {
private:
    std::unordered_map<BaseSystem::family_type, std::unique_ptr<BaseSystem>> m_systems;

public:
    /// @brief Constructs a new system S in place with args
    /// @note This function does nothing if a system with type S already exists
    /// @param args Arguments to construct S with
    template <typename S, typename... Args, std::enable_if_t<std::is_base_of_v<BaseSystem, S>, int> = 0>
    void add(Args &&... args);

    /// @brief Get and return system S
    /// @return A pointer to system S if present, otherwise nullptr
    template <typename S>
    S *get() const;

    const auto &systems() const {
        return m_systems;
    }
};

template <typename S, typename... Args, std::enable_if_t<std::is_base_of_v<BaseSystem, S>, int>>
void SystemManager::add(Args &&... args) {
    m_systems.emplace(S::family(), std::make_unique<S>(std::forward<Args>(args)...));
}

template <typename S>
S *SystemManager::get() const {
    const auto it = m_systems.find(S::family());
    return it != m_systems.end() ? static_cast<S *>(it->second.get()) : nullptr;
}

} // namespace inexor::ecs
