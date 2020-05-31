#pragma once

#include <cstdint>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <utility>

namespace inexor::ecs {

using EntityId = std::size_t;
class EntityManager;

/// @brief Wrapper class around an entity id
/// @note Doesn't contain any real data, only helper functions to modify the entity
class Entity {
    friend EntityManager;

private:
    const EntityId m_id;
    EntityManager *const m_manager;

    constexpr Entity(EntityId id, EntityManager *manager) : m_id(id), m_manager(manager) {}

public:
    /// @brief Constructs a new component C in place with args
    /// @note This function does nothing if component C already exists within the entity
    /// @param args Arguments to construct C with
    /// @return A pointer to the newly created C
    template <typename C, typename... Args>
    C *add(Args &&... args);

    /// @brief Get and return component C
    /// @return A pointer to component C if present, otherwise nullptr
    template <typename C>
    C *get() const;

    // TODO(): Add has component function
    // TODO(): Add remove component function

    /// @brief Destroys this entity
    /// @note Use of the entity after destruction is undefined
    void destroy();
};

class EntityManager {
    struct BaseComponentContainer {
        using family_type = std::size_t;

    public:
        static family_type s_family_counter;

        virtual ~BaseComponentContainer() = default;
    };

    template <typename C>
    struct ComponentContainer : public BaseComponentContainer {
        static family_type family() {
            static family_type family = s_family_counter++;
            return family;
        }

    private:
        C m_comp;

    public:
        template <typename... Args>
        constexpr explicit ComponentContainer(Args &&... args) : m_comp{std::forward<Args>(args)...} {}

        C *comp() {
            return &m_comp;
        }
    };

private:
    // unique_ptr is needed because every component has a different size
    std::unordered_map<EntityId,
                       std::unordered_map<BaseComponentContainer::family_type, std::unique_ptr<BaseComponentContainer>>>
        m_components;
    EntityId m_count;

public:
    EntityManager() : m_count(0) {}

    // TODO(): Add docs
    Entity add_entity();

    // TODO(): Add get_entity(EntityId) function

    // TODO(): Add docs
    template <typename C, typename... Args>
    C *add_to(EntityId id, Args &&... args);

    // TODO(): Add docs
    template <typename C>
    C *get_from(EntityId id) const;

    // TODO(): Add each iterator function

    // TODO(): Add docs
    void destroy_entity(EntityId id);
};

template <typename C, typename... Args>
C *Entity::add(Args &&... args) {
    return m_manager->add_to<C>(m_id, std::forward<Args>(args)...);
}

template <typename C>
C *Entity::get() const {
    return m_manager->get_from<C>(m_id);
}

template <typename C, typename... Args>
C *EntityManager::add_to(EntityId id, Args &&... args) {
    // Use of std::unordered_map::operator[] ensures presence
    auto &components = m_components[id];
    auto pair = components.emplace(ComponentContainer<C>::family(),
                                   std::make_unique<ComponentContainer<C>>(std::forward<Args>(args)...));
    return static_cast<ComponentContainer<C> *>(pair.first->second.get())->comp();
}

template <typename C>
C *EntityManager::get_from(EntityId id) const {
    // TODO(): Add assert for invalid id
    const auto &components = m_components.at(id);
    const auto it = components.find(ComponentContainer<C>::family());
    return it != components.end() ? static_cast<ComponentContainer<C> *>(it->second.get())->comp() : nullptr;
}

} // namespace inexor::ecs
