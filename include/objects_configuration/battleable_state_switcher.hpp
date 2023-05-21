#ifndef BATTLEABLE_SWITCHER_CONFIG_H
#define BATTLEABLE_SWITCHER_CONFIG_H

#include <queue>

#include <ecs/system.hpp>

#include <events/renderer_events.hpp>
#include <events/action_events.hpp>

class BattleAbleStateSwitchSystem : public ecs::System<BattleAbleStateSwitchSystem>, public ecs::Reciever<BattleAbleStateSwitchSystem>{
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const ActionCommandRequestEvent& event);
    void Receive(const StateRenderedEvent& event);

private:
    // void HandleAction(ecs::Entity object, ecs::Entity action);
private:
    std::queue<ActionCommandEvent> requests_;
    ecs::EventManager* events_;
};

#endif // BATTLEABLE_SWITCHER_CONFIG_H
