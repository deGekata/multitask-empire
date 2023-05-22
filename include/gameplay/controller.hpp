#ifndef H_CONTROLLER
#define H_CONTROLLER

#include <ecs/quick.hpp>
#include <utility/metric.hpp>

class ControllerSystem : public ecs::System<ControllerSystem> {
    enum class GameState { Init, Knight1, Knight2, Knight3, Final };

public:
    ControllerSystem();

    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

private:
    static void KnightBehaviour(ecs::Entity current, ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt);

    void SwitchGameState(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt);

    GameState current_state_;

    static metrics::TimeStorage update_timestamp_;
    static metrics::TimeStorage block_timestamp_;
};

#endif
