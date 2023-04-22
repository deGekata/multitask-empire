#include <player/player.hpp>

#include <components/movement_components.hpp>
#include <components/player_components.hpp>

void PlayerSystem::Configure(ecs::EntityManager& entities) {
    ecs::Entity player = entities.Create();
    player.Assign<PlayerTag>();

    player.Assign<Position>();
    player.Assign<Velocity>();
    player.Assign<Acceleration>();

    std::cout << "[*] Created player with index: " << player.GetId().GetIndex() << std::endl;
}

void PlayerSystem::Update(ecs::EntityManager& entities, ecs::TimeDelta) {
    std::string command;
    if (std::cin >> command) {
        entities.Each<PlayerTag>([command](ecs::Entity entity, PlayerTag&) {
            entity.AssignFromCopy<PendingPlayerCommand>({command});

            std::cout << "Assigned pending command " << command << " to entity " << entity.GetId().GetIndex()
                      << std::endl;
        });
    }
}
