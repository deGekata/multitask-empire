#ifndef H_UTILITY_CPNTS
#define H_UTILITY_CPNTS

#include <functional>

#include <ecs/entity.hpp>

#include <components/movement_components.hpp>

struct Attached {
    ecs::Entity owner;

    std::function<Position(Position)> pos_modifier;
};  

#endif
