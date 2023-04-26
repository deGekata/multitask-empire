#include <physics/friction.hpp>

#include <components/movement_components.hpp>

#include <events/movement_events.hpp>

void FrictionSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    entities.Each<Velocity>([&events](ecs::Entity, Velocity& vel) {
        if ((vel.vx_ != 0) && (std::abs(vel.vx_) <= kFrictionStopValue)) {
            vel.vx_ = 0;

            events.Emit<MovementStopEvent>();
        }
    });

    entities.Each<Velocity, Acceleration>([](ecs::Entity, Velocity& vel, Acceleration& acc) {
        acc.ax_ = -vel.vx_ / kFrictionCoefficient;
    });
}
