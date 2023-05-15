#ifndef H_COLLISION_EVENTS
#define H_COLLISION_EVENTS

#include <ecs/entity.hpp>

struct CollisionEvent : public ecs::Event<CollisionEvent> {
    explicit CollisionEvent(ecs::Entity first, ecs::Entity second) : first_collided_(first), second_collided_(second) {
    }

    CollisionEvent(const CollisionEvent&) = default;
    ~CollisionEvent() override = default;

    ecs::Entity first_collided_;
    ecs::Entity second_collided_;
};

#endif
