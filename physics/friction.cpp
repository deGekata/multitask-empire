#include <physics/friction.hpp>

#include <components/movement_components.hpp>

void FrictionSystem::Update(ecs::EntityManager& entities, ecs::TimeDelta) {
    entities.Each<Velocity>([](ecs::Entity, Velocity& vel) {
        if (std::abs(vel.vx_) <= kFrictionStopValue) {
          vel.vx_ = 0;
        }
    });
    
    entities.Each<Velocity, Acceleration>([](ecs::Entity, Velocity& vel, Acceleration& acc) {
        acc.ax_ = - vel.vx_ / kFrictionCoefficient;
    });
}
