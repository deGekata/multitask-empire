#include <physics/mv_commands.hpp>

#include <iostream>

#include <components/player_components.hpp>
#include <components/movement_components.hpp>

#include <utility/utilities.hpp>

void MovementCommandsSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
}

void MovementCommandsSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void MovementCommandsSystem::Receive(const PlayerCommandEvent& event) {
    ecs::Entity entity = event.entity_;
    
    switch (event.cmd_.type_) {
        case PlayerCommandType::Jump: {
            auto position = entity.GetComponent<Position>();
            if (IsEqual(position->y_, 0.0)) {
                auto velocity = entity.GetComponent<Velocity>();

                velocity->vy_ = kJumpSpeed;
            }
            break;
        }

        case PlayerCommandType::RunLeft: {
            auto velocity = entity.GetComponent<Velocity>();
            velocity->vx_ = -kMoveSpeed;

            break;
        }

        case PlayerCommandType::RunRight: {
            auto velocity = entity.GetComponent<Velocity>();
            velocity->vx_ = kMoveSpeed;
            break;
        }

        case PlayerCommandType::StopRunningLeft: {
            
            if (IsEqual(entity.GetComponent<Position>()->y_, 0.0)) {
                if(entity.GetComponent<Velocity>()->vx_ < 0)
                    entity.GetComponent<Velocity>()->vx_ = 0.0;
            }
            break;
        }

        case PlayerCommandType::StopRunningRight: {
            
            if (IsEqual(entity.GetComponent<Position>()->y_, 0.0)) {
                if(entity.GetComponent<Velocity>()->vx_ > 0)
                    entity.GetComponent<Velocity>()->vx_ = 0.0;
            }
            break;
        }

        default: {
            break;
        }
    }
}
