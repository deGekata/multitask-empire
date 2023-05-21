#ifndef H_COLLECTOR
#define H_COLLECTOR

#include <ecs/quick.hpp>

class GarbageCollectorSystem : public ecs::System<GarbageCollectorSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
};

#endif
