#ifndef H_BOT_COMPONENTS
#define H_BOT_COMPONENTS

#include <functional>

#include <ecs/quick.hpp>

struct BotBehaviour {
    std::function<void(ecs::Entity, ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta)> behaviour_;
};

#endif
