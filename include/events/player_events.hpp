#ifndef H_PlAYER_EVENTS
#define H_PlAYER_EVENTS

#include <string>
#include <sstream>

#include <ecs/entity.hpp>
#include <ecs/event.hpp>

#include <components/player_components.hpp>

enum class PlayerCommand {
    INVALID = -1,
    IDLE,
    WALK_LEFT,
    WALK_RIGHT,
    ATTACK_ONE,
    ATTACK_TWO,
    DEATH,
    JUMP,
    TEXT_INSERT_REQUEST
};

struct PlayerInitiatedEvent : public ecs::Event<PlayerInitiatedEvent> {
    explicit PlayerInitiatedEvent(ecs::Entity player_entity) : entity_(player_entity) {
    }

    PlayerInitiatedEvent(const PlayerInitiatedEvent& other) = default;
    ~PlayerInitiatedEvent() override = default;

    ecs::Entity entity_;
};

struct PlayerCommandEvent : public ecs::Event<PlayerCommandEvent> {
    explicit PlayerCommandEvent(PlayerCommand cmd, ecs::Entity player_entity) : cmd_(cmd), entity_(player_entity) {
    }

    PlayerCommandEvent(const PlayerCommandEvent& other) = default;
    ~PlayerCommandEvent() override = default;

    PlayerCommand cmd_;
    ecs::Entity entity_;
};

struct PlayerTextRequestEvent : public ecs::Event<PlayerTextRequestEvent> {
    explicit PlayerTextRequestEvent(std::stringstream ss)
        : cmd_(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>()) {
    }

    PlayerTextRequestEvent(const PlayerTextRequestEvent& other) = default;
    ~PlayerTextRequestEvent() override = default;

    std::vector<std::string> cmd_;
};

#endif
