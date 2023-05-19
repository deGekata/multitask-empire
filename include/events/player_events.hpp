#ifndef H_PlAYER_EVENTS
#define H_PlAYER_EVENTS

#include <string>
#include <sstream>

#include <ecs/entity.hpp>
#include <ecs/event.hpp>

#include <graphics/event.hpp>

#include <components/player_components.hpp>

struct PlayerInitiatedEvent : public ecs::Event<PlayerInitiatedEvent> {
    explicit PlayerInitiatedEvent(ecs::Entity player_entity) : entity_(player_entity) {
    }

    PlayerInitiatedEvent(const PlayerInitiatedEvent& other) = default;
    ~PlayerInitiatedEvent() override = default;
    
    std::string player_name_;
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

struct KeyPressedEvent : public ecs::Event<KeyPressedEvent> {
    explicit KeyPressedEvent(igraphicslib::KeyEventData data)
        : data_(data) {
    }

    igraphicslib::KeyEventData data_;
};

struct KeyReleasedEvent : public ecs::Event<KeyReleasedEvent> {
    explicit KeyReleasedEvent(igraphicslib::KeyEventData data)
        : data_(data) {
    }

    igraphicslib::KeyEventData data_;
};

#endif
