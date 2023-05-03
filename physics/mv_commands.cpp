#include <physics/mv_commands.hpp>

#include <iostream>

#include <components/player_components.hpp>
#include <components/movement_components.hpp>

void MovementCommandsSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
}

void MovementCommandsSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
    for (auto event = events_queue_.front(); !events_queue_.empty(); events_queue_.pop_front()) {
        switch (event.cmd_) {
            case PlayerCommand::JUMP: {
                auto position = event.entity_.GetComponent<Position>();
                if (position->y_ == 0) {
                    auto velocity = event.entity_.GetComponent<Velocity>();
                    velocity->vy_ = kJumpSpeed;
                }
                break;
            }

            case PlayerCommand::WALK_LEFT: {
                auto velocity = event.entity_.GetComponent<Velocity>();
                velocity->vx_ = -kMoveSpeed;

                break;
            }

            case PlayerCommand::WALK_RIGHT: {
                auto velocity = event.entity_.GetComponent<Velocity>();
                velocity->vx_ = kMoveSpeed;

                break;
            }

            case PlayerCommand::IDLE: {
                auto position = event.entity_.GetComponent<Position>();
                if (position->y_ == 0) {
                    auto velocity = event.entity_.GetComponent<Velocity>();
                    velocity->vx_ = 0;
                }
                break;
            }

            default: {
                break;
            }
        }
    }
}

void MovementCommandsSystem::Recieve(const PlayerCommandEvent& event) {
    events_queue_.push_back(event);
}
