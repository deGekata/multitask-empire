#ifndef H_BOT
#define H_BOT

#include <map>
#include <queue>

#include <events/bot_events.hpp>
#include <ecs/system.hpp>

class BotSystem : public ecs::System<BotSystem>, public ecs::Receiver<BotSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const SpawnBotEvent& event);

private:
    void ProcessQueue(ecs::EntityManager& entities, ecs::EventManager& events);

    std::map<std::string, int> state_name_to_player_state_id_;
    std::queue<SpawnBotEvent> spawn_queue_;
};

#endif
