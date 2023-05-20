#include <bot/bot.hpp>

#include <events/player_events.hpp>
#include <events/renderer_events.hpp>

#include <components/bot_components.hpp>
#include <components/movement_components.hpp>
#include <components/player_components.hpp>

#include <player/player.hpp>

void BotSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<SpawnBotEvent>(*this);
}

void BotSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
    ProcessQueue(entities, events);

    entities.Each<BotBehaviour>([&entities, &events, dt](ecs::Entity entity, BotBehaviour& fun) {
        fun.behaviour_(entity, entities, events, dt);
    });
}

void BotSystem::ProcessQueue(ecs::EntityManager& entities, ecs::EventManager& events) {
    while (!spawn_queue_.empty()) {
        auto spawn_event = spawn_queue_.front();
        spawn_queue_.pop();

        // Create new bot and start tracking it
        ecs::Entity bot = entities.Create();
        entities.Tracker().TrackEntity(bot.GetId().GetIndex());
        events.Emit<PlayerInitiatedEvent>(bot);

        bot.Assign<BotBehaviour>(BotBehaviour{std::move(spawn_event.behaviour_)});
        if (bot.HasComponent<Position>()) {
            *bot.GetComponent<Position>() = spawn_event.spawn_position_;
        }

        events.Emit<BattleAbleConfigChangeRequest>(spawn_event.bot_config_name_, bot);
        logger::Print("Set bot's skin\n");

        // events.Emit<PlayerCommandEvent>(PlayerCommand::IDLE, bot);
        // logger::Print("Idle event sent\n");
    }
}

void BotSystem::Receive(const SpawnBotEvent& event) {
    spawn_queue_.push(event);
}
