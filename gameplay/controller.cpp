#include <gameplay/controller.hpp>

#include <events/bot_events.hpp>
#include <events/player_events.hpp>

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

    if ((time_since += dt) >= 1000) {
        events.Emit<PlayerCommandEvent>(PlayerCommand::JUMP, current);

        time_since = 0;
    }
}

void ControllerSystem::SwitchGameState(ecs::EntityManager&, ecs::EventManager& events, ecs::TimeDelta) {
    switch (current_state_) {
        case GameState::Init:
            events.Emit<SpawnBotEvent>(Position{500, 0}, "./assets/sprites/knight.png",
                                       &ControllerSystem::KnightBehaviour);

            break;

        case GameState::Knight:
            current_state_ = GameState::Final;
            break;

        case GameState::Final:
            break;
    }
}
