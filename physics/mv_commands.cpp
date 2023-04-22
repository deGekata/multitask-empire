#include <physics/mv_commands.hpp>

#include <iostream>

#include <components/player_components.hpp>
#include <components/movement_components.hpp>

void MovementCommandsSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PendingMovementEvent>(*this);
}

void MovementCommandsSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
    for (auto event = events_queue_.front(); !events_queue_.empty(); events_queue_.pop_front()) {
        switch (event.type_) {
            case MovementCommand::Jump: {
                auto position = event.target_.GetComponent<Position>();
                if (position->y_ == 0) {
                    auto velocity = event.target_.GetComponent<Velocity>();
                    velocity->vy_ = kJumpSpeed;
                }
                break;
            }

            case MovementCommand::Left: {
                auto velocity = event.target_.GetComponent<Velocity>();
                velocity->vx_ = -kMoveSpeed;

                break;
            }

            case MovementCommand::Right: {
                auto velocity = event.target_.GetComponent<Velocity>();
                velocity->vx_ = kMoveSpeed;

                break;
            }
        }
    }
}

void MovementCommandsSystem::Recieve(const PendingMovementEvent& event) {
    events_queue_.push_back(event);
}
