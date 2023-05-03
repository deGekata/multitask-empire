#include <player/player.hpp>

#include <events/player_events.hpp>

#include <components/movement_components.hpp>
#include <components/player_components.hpp>

void PlayerSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    ecs::Entity player = entities.Create();
    player.Assign<PlayerTag>();

    player.Assign<Position>();
    player.Assign<Velocity>();
    player.Assign<Acceleration>();

    entities.Tracker().TrackEntity(player.GetId().GetIndex());
    std::cout << "[*] Created player with index: " << player.GetId().GetIndex() << std::endl;

    events.Emit<PlayerInitiatedEvent>(player);
    
    //! remove, use only for debug
    events.Emit<SpriteSheetLoadRequest>("orc_berserk.xml");
    events.Emit<SkinChangeRequest>("./orc_berserk.png", player);

    events.Emit<PlayerCommandEvent>(PLAYER_CMD::IDLE, player);
}

void PlayerSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
    // std::string command;
    // if (std::cin >> command) {
    //     PlayerCommandEvent cmd{command};

    //     events.Emit<PlayerCommandEvent>(std::move(cmd));
    // }
}
