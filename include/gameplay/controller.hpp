#ifndef H_CONTROLLER
#define H_CONTROLLER

#include <ecs/quick.hpp>

class ControllerSystem : public ecs::System<ControllerSystem> {
    enum class GameState { Init, Knight, Final };

public:
    ControllerSystem();

    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

private:
    static void KnightBehaviour(ecs::Entity current, ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt);

    void SwitchGameState(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt);

    GameState current_state_;
};

#endif
