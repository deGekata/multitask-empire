#ifndef H_MOVEMENT
#define H_MOVEMENT

#include <ecs/system.hpp>
#include <chrono>

class MovementSystem : public ecs::System<MovementSystem> {
public:
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
private:
    std::chrono::high_resolution_clock::time_point init_time_;
};

#endif
