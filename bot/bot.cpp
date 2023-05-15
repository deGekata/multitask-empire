#include <bot/bot.hpp>

#include <events/player_events.hpp>
#include <events/renderer_events.hpp>

#include <components/bot_components.hpp>
#include <components/movement_components.hpp>
#include <components/player_components.hpp>

static void FillStatesNameMap(std::map<std::string, int>* storage);

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
    if (spawn_queue_.empty()) {
        return;
    }

    std::map<std::string, int> state_name_to_player_state_id;
    FillStatesNameMap(&state_name_to_player_state_id);

    for (auto spawn_event = spawn_queue_.front(); !spawn_queue_.empty(); spawn_queue_.pop()) {
        ecs::Entity bot = entities.Create();

        entities.Tracker().TrackEntity(bot.GetId().GetIndex());

        //! remove
        events.Emit<PlayerInitiatedEvent>(bot);
        logger::Print("Initialized bot\n");

        if (bot.HasComponent<Position>()) {
            *bot.GetComponent<Position>() = spawn_event.spawn_position_;
        }
        logger::Print("Set bot's position\n");

        bot.Assign<BotBehaviour>(BotBehaviour{std::move(spawn_event.behaviour_)});
        logger::Print("Set bot's behaviour\n");

        events.Emit<SkinChangeRequest>(state_name_to_player_state_id, static_cast<int>(PlayerCommand::IDLE), spawn_event.bot_skin_, bot);
        logger::Print("Set bot's skin\n");

        // events.Emit<PlayerCommandEvent>(PlayerCommand::IDLE, bot);
        // logger::Print("Idle event sent\n");
    }
}

void BotSystem::Recieve(const SpawnBotEvent& event) {
    spawn_queue_.push(event);
}

#define ADD_CMD_STR_MATCH(cmd) storage->operator[](#cmd) = static_cast<int>(PlayerCommand::cmd);

static void FillStatesNameMap(std::map<std::string, int>* storage) {
    ADD_CMD_STR_MATCH(IDLE)
    ADD_CMD_STR_MATCH(WALK_LEFT)
    ADD_CMD_STR_MATCH(WALK_RIGHT)
    ADD_CMD_STR_MATCH(ATTACK_ONE)
    ADD_CMD_STR_MATCH(ATTACK_TWO)
    ADD_CMD_STR_MATCH(DEATH)
    ADD_CMD_STR_MATCH(JUMP)    
}
