#ifndef H_ACTION_EVENTS
#define H_ACTION_EVENTS

#include <ecs/entity.hpp>

#include <components/player_components.hpp>

struct ActionCommandEvent : public ecs::Event<ActionCommandEvent> {
    explicit ActionCommandEvent(ecs::Entity action, ecs::Entity obj_entity) : action_(action), obj_entity_(obj_entity) {
    }

    ActionCommandEvent(const ActionCommandEvent& other) = default;
    ~ActionCommandEvent() override = default;

    ecs::Entity action_;
    ecs::Entity obj_entity_;
};

struct ActionCommandRequestEvent : public ecs::Event<ActionCommandRequestEvent> {
    explicit ActionCommandRequestEvent(ecs::Entity action, ecs::Entity obj_entity) : action_(action) , obj_entity_(obj_entity){
    }

    ActionCommandRequestEvent(const ActionCommandRequestEvent& other) = default;
    ~ActionCommandRequestEvent() override = default;

    ecs::Entity action_;
    ecs::Entity obj_entity_;
};

#endif // H_ACTION_EVENTS
