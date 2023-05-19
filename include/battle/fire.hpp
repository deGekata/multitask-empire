#ifndef H_FIRE
#define H_FIRE

#include <map>
#include <string>
#include <queue>

#include <ecs/quick.hpp>

#include <events/battle_events.hpp>

class FireSystem : public ecs::System<FireSystem>, public ecs::Reciever<FireSystem> {
    enum MissleStates : int { FLYING };

public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const SpecialTriggerEvent& event);

private:
    void ProcessFires(ecs::EntityManager& entities, ecs::EventManager& events);

    std::map<std::string, int> state_name_converter_;
    std::queue<ecs::Entity> fires_queue_;
};

#endif
