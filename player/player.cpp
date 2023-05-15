#include <player/player.hpp>

#include <events/player_events.hpp>
#include <events/renderer_events.hpp>

#include <components/movement_components.hpp>
#include <components/player_components.hpp>

void PlayerSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    ecs::Entity player = entities.Create();
    player.Assign<PlayerTag>();

    entities.Tracker().TrackEntity(player.GetId().GetIndex());

    //! remove
    events.Emit<PlayerInitiatedEvent>(player);
    events.Emit<SkinChangeRequest>("./assets/sprites/orc_berserk.png", player);
    events.Emit<PlayerCommandEvent>(PlayerCommand::IDLE, player);
}

void PlayerSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}
