#include <physics/movement.hpp>

#include <iostream>

#include <components/movement_components.hpp>

#include <events/gravitation_events.hpp>
#include <events/movement_events.hpp>

void MovementSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
    entities.Each<Position, Velocity, Acceleration>(
        [dt](ecs::Entity, Position& cords, Velocity& vel, Acceleration& acc) {
            if (cords.y_ == 0) {
                vel.vx_ += acc.ax_ * dt;
            }
            vel.vy_ += acc.ay_ * dt;
        });

    entities.Each<Position, Velocity>([dt, &events](ecs::Entity entity, Position& pos, Velocity& vel) {
        pos.y_ = std::max(0l, pos.y_ + vel.vy_ * dt);
        pos.x_ += vel.vx_ * dt;

        if ((pos.y_ == 0) && (vel.vy_ != 0)) {
            LandingEvent landing_event{entity};
            events.Emit<LandingEvent>(landing_event);

            if (vel.vx_ == 0) {
                events.Emit<MovementStopEvent>();
            }
        }
    });
}
