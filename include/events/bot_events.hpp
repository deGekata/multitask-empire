#ifndef H_BOT_EVENTS
#define H_BOT_EVENTS

#include <functional>
#include <string>

#include <ecs/quick.hpp>

#include <components/movement_components.hpp>

struct SpawnBotEvent : public ecs::Event<SpawnBotEvent> {
    using Behaviour = std::function<void(ecs::Entity, ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta)>;

    SpawnBotEvent(Position pos, std::string skin, Behaviour behaviour)
        : spawn_position_(pos), bot_config_name_(skin), behaviour_(std::move(behaviour)) {
    }

    Position spawn_position_;
    std::string bot_config_name_;

    Behaviour behaviour_;
};

#endif
