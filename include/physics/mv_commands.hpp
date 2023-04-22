#ifndef H_MOVEMENT_COMMANDS
#define H_MOVEMENT_COMMANDS

#include <ecs/system.hpp>

const int64_t kJumpSpeed = 100;
const int64_t kMoveSpeed = 100;

class MovementCommandsSystem : public ecs::System<MovementCommandsSystem> {
public:
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
};

#endif
