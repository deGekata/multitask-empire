#include <gameplay/controller.hpp>

#include <events/bot_events.hpp>
#include <events/player_events.hpp>

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
    // static ecs::TimeDelta time_since = 0;
    // static bool move_direction = false;

    // if ((time_since += dt) >= 1000000.0) {
    //     move_direction = !move_direction;

    //     time_since = 0;
    // }

    // if (move_direction) {
    //     events.Emit<PlayerCommandEvent>(PlayerCommand::WALK_LEFT, current);
    // } else {
    //     events.Emit<PlayerCommandEvent>(PlayerCommand::WALK_RIGHT, current);
    // }
}

void ControllerSystem::SwitchGameState(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    switch (current_state_) {
        case GameState::Init:
            entities.Each<PlayerTag>([](ecs::Entity entity, PlayerTag&) {
                entity.Assign<SpecialAbility>(SpecialAbility{SpecialAbility::Type::Fireball});
            });

            current_state_ = GameState::Knight;
            break;

        case GameState::Knight:
            events.Emit<SpawnBotEvent>(Position{500, 0}, "./assets/sprites/knight.png",
                                       &ControllerSystem::KnightBehaviour);
            break;

        case GameState::Final:
            break;
    }
}
