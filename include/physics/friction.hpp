#ifndef H_FRICTION
#define H_FRICTION

#include <ecs/system.hpp>
#include <physics/movement.hpp>

constexpr int64_t kFrictionCoefficient = 3;
constexpr double kFrictionStopValue = 10.0;

class FrictionSystem : public ecs::System<FrictionSystem> {
public:
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
};

#endif
