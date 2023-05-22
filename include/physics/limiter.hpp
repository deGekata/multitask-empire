#ifndef H_LIMITER
#define H_LIMITER

#include <ecs/quick.hpp>

class LimiterSystem : public ecs::System<LimiterSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
};

#endif
