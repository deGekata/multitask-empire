#include <physics/mv_commands.hpp>

#include <iostream>

#include <components/player_components.hpp>
#include <components/movement_components.hpp>

void MovementCommandsSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
}

void MovementCommandsSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void MovementCommandsSystem::Recieve(const PlayerCommandEvent& event) {
    ecs::Entity entity = event.entity_;

    switch (event.cmd_) {
        case PlayerCommand::JUMP: {
            auto position = entity.GetComponent<Position>();
            if (position->y_ == 0) {
                auto velocity = entity.GetComponent<Velocity>();
                velocity->vy_ = kJumpSpeed;
            }
            break;
        }

        case PlayerCommand::WALK_LEFT: {
            auto velocity = entity.GetComponent<Velocity>();
            velocity->vx_ = -kMoveSpeed;

            break;
        }

        case PlayerCommand::WALK_RIGHT: {
            auto velocity = entity.GetComponent<Velocity>();
            velocity->vx_ = kMoveSpeed;

            break;
        }

        case PlayerCommand::IDLE: { 
            if (entity.GetComponent<Position>()->y_ == 0) {
                entity.GetComponent<Velocity>()->vx_ = 0;
            }
            break;
        }

        default: {
            break;
        }
    }
}
