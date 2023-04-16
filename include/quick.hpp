#pragma once

#include "config.hpp"
#include "entity.hpp"
#include "system.hpp"

namespace ecs {
class ECS {
public:
    ECS();

    EntityManager entities_;
    SystemManager systems_;
};
};  // namespace ecs