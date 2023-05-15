#ifndef H_BOT
#define H_BOT

#include <queue>

#include <events/bot_events.hpp>
#include <ecs/system.hpp>

class BotSystem : public ecs::System<BotSystem>, public ecs::Reciever<BotSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const SpawnBotEvent& event);

private:
    void ProcessQueue(ecs::EntityManager& entities, ecs::EventManager& events);

    std::queue<SpawnBotEvent> spawn_queue_;
};

#endif
