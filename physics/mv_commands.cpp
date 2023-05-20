#include <physics/mv_commands.hpp>

#include <iostream>

#include <components/player_components.hpp>
#include <components/movement_components.hpp>

#include <utility/utilities.hpp>

void MovementCommandsSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
    events.Subscribe<PlayerInitiatedEvent>(*this);
}

void MovementCommandsSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void MovementCommandsSystem::Receive(const PlayerCommandEvent& event) {
    ecs::Entity entity = event.entity_;

    switch (event.cmd_) {
        case PlayerCommand::JUMP: {
            auto position = entity.GetComponent<Position>();
            if (IsEqual(position->y_, 0.0)) {
                auto velocity = entity.GetComponent<Velocity>();
                auto jump_speed = entity.GetComponent<JumpSpeed>();

                velocity->vy_ = jump_speed->value_;
            }
            break;
        }

        case PlayerCommand::WALK_LEFT: {
            auto velocity = entity.GetComponent<Velocity>();
            auto speed = entity.GetComponent<MoveSpeed>();

            velocity->vx_ = -speed->value_;

            break;
        }

        case PlayerCommand::WALK_RIGHT: {
            auto velocity = entity.GetComponent<Velocity>();
            auto speed = entity.GetComponent<MoveSpeed>();

            velocity->vx_ = speed->value_;

            break;
        }

        case PlayerCommand::IDLE: {
            if (IsEqual(entity.GetComponent<Position>()->y_, 0.0)) {
                entity.GetComponent<Velocity>()->vx_ = 0.0;
            }
            break;
        }

        default: {
            break;
        }
    }
}

void MovementCommandsSystem::Receive(const PlayerInitiatedEvent& event) {
    ecs::Entity player = event.entity_;

    player.Assign<MoveSpeed>(MoveSpeed{kDefaultMoveSpeed});
    player.Assign<JumpSpeed>(JumpSpeed{kDefaultJumpSpeed});
}
