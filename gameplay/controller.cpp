#include <gameplay/controller.hpp>

#include <events/bot_events.hpp>
#include <events/player_events.hpp>
#include <events/action_events.hpp>

#include <components/battle_components.hpp>
#include <components/bot_components.hpp>
#include <components/player_components.hpp>

ControllerSystem::ControllerSystem() : current_state_(GameState::Init) {
}

void ControllerSystem::Configure(ecs::EntityManager&, ecs::EventManager&) {
}

void ControllerSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
    uint32_t enemies_counter = 0;
    entities.Each<BotBehaviour>([&enemies_counter](ecs::Entity, BotBehaviour&) { enemies_counter++; });

    if (enemies_counter == 0) {
        SwitchGameState(entities, events, dt);
    }
}

void ControllerSystem::KnightBehaviour(ecs::Entity current, ecs::EntityManager&, ecs::EventManager& events,
                                       ecs::TimeDelta dt) {
    static ecs::TimeDelta time_since = 0;
    static bool move_direction = false;

    if ((time_since += dt) >= 1000000.0) {
        move_direction = !move_direction;

        time_since = 0;

    if (move_direction) {

        ecs::Entity cmd_ent = entities.Create();
        PlayerCommand cmd = {.type_ = PlayerCommandType::Action};
        cmd_ent.Assign<PlayerCommand>(cmd);

        ActionCommand action_cmd = {.type_ = ActionCommandType::RunLeft};
        cmd_ent.Assign<ActionCommand>(action_cmd);

        events.Emit<ActionCommandRequestEvent>(cmd_ent, current);
        entities.Destroy(cmd_ent.GetId());
    }
    else {

        ecs::Entity cmd_ent = entities.Create();
        PlayerCommand cmd = {.type_ = PlayerCommandType::Action};
        cmd_ent.Assign<PlayerCommand>(cmd);

        ActionCommand action_cmd = {.type_ = ActionCommandType::RunRight};
        cmd_ent.Assign<ActionCommand>(action_cmd);

        events.Emit<ActionCommandRequestEvent>(cmd_ent, current);
        entities.Destroy(cmd_ent.GetId());
    }

    }
}

void ControllerSystem::SwitchGameState(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    switch (current_state_) {
        case GameState::Init:
            entities.Each<PlayerTag>([](ecs::Entity entity, PlayerTag&) {
                entity.Assign<SpecialAbility>(SpecialAbility{SpecialAbility::Type::Potion});
            });

            current_state_ = GameState::Knight;
            break;

        case GameState::Knight:
            events.Emit<SpawnBotEvent>(Position{500, 0}, "./knight.wtf",
                                       &ControllerSystem::KnightBehaviour);
            break;

        case GameState::Final:
            break;
    }
}
