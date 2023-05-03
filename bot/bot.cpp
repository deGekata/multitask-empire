#include <bot/bot.hpp>

#include <events/player_events.hpp>

#include <components/movement_components.hpp>
#include <components/player_components.hpp>

void BotSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    ecs::Entity bot = entities.Create();

    entities.Tracker().TrackEntity(bot.GetId().GetIndex());

    //! remove
    events.Emit<PlayerInitiatedEvent>(bot);
    if (bot.HasComponent<Position>()) {
        bot.GetComponent<Position>()->x_ = 500;
    }

    events.Emit<SkinChangeRequest>("./assets/sprites/knight.png", bot);
    events.Emit<PlayerCommandEvent>(PlayerCommand::IDLE, bot);
}

void BotSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}
