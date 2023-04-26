#include <player/dispatcher.hpp>

#include <iostream>

#include <components/movement_components.hpp>
#include <components/player_components.hpp>

#include <events/movement_events.hpp>

DispatcherSystem::DispatcherSystem(bool& flag) : running_(flag) {
}

void DispatcherSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
}

void DispatcherSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    entities.Each<PlayerTag>([this, &events](ecs::Entity entity, PlayerTag&) {
        for (auto cmd = events_queue_.front(); !events_queue_.empty(); events_queue_.pop_front()) {
            if (cmd.command_ == "JUMP") {
                PendingMovementEvent event{entity, MovementCommand::Jump};
                events.Emit<PendingMovementEvent>(event);
            } else if (cmd.command_ == "LEFT") {
                PendingMovementEvent event{entity, MovementCommand::Left};
                events.Emit<PendingMovementEvent>(event);
            } else if (cmd.command_ == "RIGHT") {
                PendingMovementEvent event{entity, MovementCommand::Right};
                events.Emit<PendingMovementEvent>(event);
            } else if (cmd.command_ == "SKIP") {
            } else if (cmd.command_ == "EXIT") {
                running_ = false;
            }
        }
    });
}

void DispatcherSystem::Recieve(const PlayerCommandEvent& cmd) {
    events_queue_.push_back(cmd);
}
