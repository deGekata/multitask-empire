#ifndef H_PlAYER_EVENTS
#define H_PlAYER_EVENTS

#include <string>
#include <sstream>

#include <ecs/entity.hpp>
#include <ecs/event.hpp>

#include <graphics/event.hpp>

#include <components/player_components.hpp>

enum class PlayerCommand : int {
    INVALID = -1,
    IDLE,
    WALK_LEFT,
    WALK_RIGHT,
    ATTACK_ONE,
    ATTACK_TWO,
    DEATH,
    JUMP,
    FIRE,
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

struct MouseMovedEvent : public ecs::Event<MouseMovedEvent> {
    explicit MouseMovedEvent(igraphicslib::MouseMoveEventData data)
        : mouse_data(data) {
    }

    igraphicslib::MouseMoveEventData mouse_data;
};

struct MouseButtonPressedEvent : public ecs::Event<MouseButtonPressedEvent> {
    explicit MouseButtonPressedEvent(igraphicslib::MouseButtonEventData data)
        : button_data(data) {
    }

    igraphicslib::MouseButtonEventData button_data;
};

struct MouseButtonReleasedEvent : public ecs::Event<MouseButtonReleasedEvent> {
    explicit MouseButtonReleasedEvent(igraphicslib::MouseButtonEventData data)
        : button_data(data) {
    }

    igraphicslib::MouseButtonEventData button_data;
};

struct MouseWheelScrolledEvent : public ecs::Event<MouseWheelScrolledEvent> {
    explicit MouseWheelScrolledEvent(igraphicslib::MouseWheelEventData data)
        : wheel_data(data) {
    }

    igraphicslib::MouseWheelEventData wheel_data;
};

struct TimerTickedEvent : public ecs::Event<TimerTickedEvent> {
    explicit TimerTickedEvent(igraphicslib::Event::Timepoint data)
        : time_data(data) {
    }

    igraphicslib::Event::Timepoint time_data;
};

#endif
