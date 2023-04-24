#ifndef H_MOVEMENT
#define H_MOVEMENT

#include <ecs/system.hpp>

class MovementSystem : public ecs::System<MovementSystem> {
public:
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
};

#endif