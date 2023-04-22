#include <player/dispatcher.hpp>

#include <iostream>

#include <components/movement_components.hpp>
#include <components/player_components.hpp>

DispatcherSystem::DispatcherSystem(bool& flag) : running_(flag) {
}

void DispatcherSystem::Update(ecs::EntityManager& entities, ecs::TimeDelta) {
    entities.Each<PlayerTag, PendingPlayerCommand>([this](ecs::Entity entity, PlayerTag&, PendingPlayerCommand& cmd) {
        if (cmd.command_ == "JUMP") {
            entity.Remove<PendingPlayerCommand>();
            entity.Assign<PendingMovementCommand>(PendingMovementCommand::Jump);

            std::cout << "Caught UP command" << std::endl;
        } else if (cmd.command_ == "LEFT") {
            entity.Remove<PendingPlayerCommand>();
            entity.Assign<PendingMovementCommand>(PendingMovementCommand::Left);
        } else if (cmd.command_ == "RIGHT") {
            entity.Remove<PendingPlayerCommand>();
            entity.Assign<PendingMovementCommand>(PendingMovementCommand::Right);
        } else if (cmd.command_ == "SKIP") {
            entity.Remove<PendingPlayerCommand>();
        } else if (cmd.command_ == "EXIT") {
            entity.Remove<PendingPlayerCommand>();
            running_ = false;
        }
    });
}
