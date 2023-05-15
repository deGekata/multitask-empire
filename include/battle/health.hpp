#ifndef H_HEALTH
#define H_HEALTH

#include <events/collision_events.hpp>

#include <ecs/quick.hpp>

class HealthSystem : public ecs::System<HealthSystem>, public ecs::Reciever<HealthSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Recieve(const CollisionEvent& event);

private:
};

#endif
