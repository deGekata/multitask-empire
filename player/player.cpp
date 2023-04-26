#include <player/player.hpp>

#include <events/player_events.hpp>

#include <components/movement_components.hpp>
#include <components/player_components.hpp>

void PlayerSystem::Configure(ecs::EntityManager& entities, ecs::EventManager&) {
    ecs::Entity player = entities.Create();
    player.Assign<PlayerTag>();

    player.Assign<Position>();
    player.Assign<Velocity>();
    player.Assign<Acceleration>();

    std::cout << "[*] Created player with index: " << player.GetId().GetIndex() << std::endl;
}

void PlayerSystem::Update(ecs::EntityManager&, ecs::EventManager& events, ecs::TimeDelta) {
    std::string command;
    if (std::cin >> command) {
        PlayerCommandEvent cmd{command};

        events.Emit<PlayerCommandEvent>(std::move(cmd));
    }
}
