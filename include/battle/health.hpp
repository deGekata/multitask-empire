#ifndef H_HEALTH
#define H_HEALTH

#include <events/battle_events.hpp>
#include <events/collision_events.hpp>

#include <ecs/quick.hpp>

class HealthSystem : public ecs::System<HealthSystem>, public ecs::Receiver<HealthSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const CollisionEvent& event);
    void Receive(const DamageTakenEvent& event);

private:
    ecs::EventManager* events_;
};

#endif
