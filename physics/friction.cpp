#include <physics/friction.hpp>

#include <components/movement_components.hpp>

#include <events/movement_events.hpp>

#include <utility/utilities.hpp>

void FrictionSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
    // entities.Each<Velocity>([&events](ecs::Entity, Velocity& vel) {
    //     if ((!IsEqual(vel.vx_, 0.0)) && (std::abs(vel.vx_) <= kFrictionStopValue)) {
    //         vel.vx_ = 0.0;

    //         events.Emit<MovementStopEvent>();
    //     }
    // });

    // entities.Each<Velocity, Acceleration>([](ecs::Entity, Velocity& vel, Acceleration& acc) {
    //     acc.ax_ = -vel.vx_ / kFrictionCoefficient;
    // });
}
