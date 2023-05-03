#ifndef H_BOT
#define H_BOT

#include <ecs/system.hpp>

class BotSystem : public ecs::System<BotSystem> {
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
};

#endif
