#include <physics/mv_commands.hpp>

#include <iostream>

#include <components/player_components.hpp>
#include <components/movement_components.hpp>

#include <utility/utilities.hpp>

void MovementCommandsSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
}

void MovementCommandsSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

// todo: to action command
void MovementCommandsSystem::Receive(const PlayerCommandEvent& event) {

    ecs::Entity cmd_ent = event.cmd_;

    auto cmd_type = cmd_ent.GetComponent<PlayerCommand>().Get();
    if(cmd_type->type_ != PlayerCommandType::Action) return;

    auto action_type = cmd_ent.GetComponent<ActionCommand>().Get();

    ecs::Entity entity = event.entity_;
    
    switch (action_type->type_) {
        case ActionCommandType::Jump: {
            auto position = entity.GetComponent<Position>();
            if (IsEqual(position->y_, 0.0)) {
                auto velocity = entity.GetComponent<Velocity>();

                velocity->vy_ = kJumpSpeed;
            }
            break;
        }

        case ActionCommandType::RunLeft: {
            auto velocity = entity.GetComponent<Velocity>();
            velocity->vx_ = -kMoveSpeed;

            break;
        }

        case ActionCommandType::RunRight: {
            auto velocity = entity.GetComponent<Velocity>();
            velocity->vx_ = kMoveSpeed;
            break;
        }

        case ActionCommandType::StopRunningLeft: {
            
            // if (IsEqual(entity.GetComponent<Position>()->y_, 0.0)) {
                if(entity.GetComponent<Velocity>()->vx_ < 0)
                    entity.GetComponent<Velocity>()->vx_ = 0.0;
            // }
            break;
        }

        case ActionCommandType::StopRunningRight: {
            
            // if (IsEqual(entity.GetComponent<Position>()->y_, 0.0)) {
                if(entity.GetComponent<Velocity>()->vx_ > 0)
                    entity.GetComponent<Velocity>()->vx_ = 0.0;
            // }
            break;
        }

        default: {
            break;
        }
    }
}
