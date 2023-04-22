#include <physics/mv_commands.hpp>

#include <iostream>

#include <components/player_components.hpp>
#include <components/movement_components.hpp>

void MovementCommandsSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta) {
    entities.Each<PendingMovementCommand, Position, Velocity>(
        [](ecs::Entity entity, PendingMovementCommand& cmd, Position& pos, Velocity& vel) {
            switch (cmd) {

                // TODO Emit jump event
                case PendingMovementCommand::Jump:
                    if (pos.y_ == 0) {
                        vel.vy_ = kJumpSpeed;
                    }
                    break;

                case PendingMovementCommand::Left:
                    vel.vx_ = -kMoveSpeed;
                    break;

                case PendingMovementCommand::Right:
                    vel.vx_ = kMoveSpeed;
                    break;
            }

            entity.Remove<PendingMovementCommand>();
        });
}
