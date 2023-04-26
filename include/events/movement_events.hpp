#ifndef H_MOVEMENT_EVENTS
#define H_MOVEMENT_EVENTS

#include <ecs/entity.hpp>

enum class MovementCommand {
    Left,
    Right,
    Jump,
};

struct PendingMovementEvent {
    ecs::Entity target_;
    MovementCommand type_;
};

struct MovementStopEvent {
    ecs::Entity target_;
};

#endif
