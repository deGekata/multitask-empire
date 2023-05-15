#ifndef H_MOVEMENT_COLLISION
#define H_MOVEMENT_COLLISION

#include <ecs/event.hpp>
#include <ecs/system.hpp>

#include <events/collision_events.hpp>

class CollisionStopperSystem : public ecs::System<CollisionStopperSystem>,
                               public ecs::Reciever<CollisionStopperSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Recieve(const CollisionEvent& collided_pair); 
};

#endif
