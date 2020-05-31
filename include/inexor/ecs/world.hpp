#pragma once

#include "inexor/ecs/entity.hpp"
#include "inexor/ecs/system.hpp"

namespace inexor::ecs {

struct World : public EntityManager, public SystemManager {
    // TODO(): Add docs
    void update(float delta_time);
};

} // namespace inexor::ecs
