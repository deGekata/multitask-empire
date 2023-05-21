#include <physics/mv_commands.hpp>

#include <iostream>

#include <components/player_components.hpp>
#include <components/movement_components.hpp>

#include <utility/utilities.hpp>

static constexpr double kDefaultJumpSpeed = 0.004;
static constexpr double kDefaultMoveSpeed = 0.0005;

void MovementCommandsSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<ActionCommandEvent>(*this);
    events.Subscribe<PlayerInitiatedEvent>(*this);
}

void MovementCommandsSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

// todo: to action command
void MovementCommandsSystem::Receive(const ActionCommandEvent& event) {

    ecs::Entity cmd_ent = event.action_;

    auto action_type = cmd_ent.GetComponent<ActionCommand>().Get();

    ecs::Entity entity = event.obj_entity_;
    
    switch (action_type->type_) {
        case ActionCommandType::Jump: {
            auto position = entity.GetComponent<Position>();
            if (IsEqual(position->y_, 0.0)) {
                auto velocity = entity.GetComponent<Velocity>();
                auto jump_speed = entity.GetComponent<JumpSpeed>();

                velocity->vy_ = jump_speed->value_;
            }
            break;
        }

        case ActionCommandType::RunLeft: {
            auto velocity = entity.GetComponent<Velocity>();
            auto speed = entity.GetComponent<MoveSpeed>();

            velocity->vx_ = -speed->value_;

            break;
        }

        case ActionCommandType::RunRight: {
            auto velocity = entity.GetComponent<Velocity>();
            auto speed = entity.GetComponent<MoveSpeed>();

            velocity->vx_ = speed->value_;
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

void MovementCommandsSystem::Receive(const PlayerInitiatedEvent& event) {
    ecs::Entity player = event.entity_;

    player.Assign<MoveSpeed>(MoveSpeed{kDefaultMoveSpeed});
    player.Assign<JumpSpeed>(JumpSpeed{kDefaultJumpSpeed});
}
