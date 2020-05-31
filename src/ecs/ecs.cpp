#include "inexor/ecs/world.hpp"

namespace inexor::ecs {

BaseSystem::family_type BaseSystem::s_family_counter;
EntityManager::BaseComponentContainer::family_type EntityManager::BaseComponentContainer::s_family_counter;

void Entity::destroy() {
    m_manager->destroy_entity(m_id);
}

Entity EntityManager::add_entity() {
    return {m_count++, this};
}

void EntityManager::destroy_entity(EntityId id) {
    m_components.erase(m_components.find(id));
}

void World::update(float delta_time) {
    for (const auto &system : systems()) {
        system.second->update(delta_time);
    }
}

} // namespace inexor::ecs
