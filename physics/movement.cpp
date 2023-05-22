#include <physics/movement.hpp>

#include <components/movement_components.hpp>

#include <events/gravitation_events.hpp>
#include <events/movement_events.hpp>

#include <utility/utilities.hpp>

void MovementSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerInitiatedEvent>(*this);
}

void MovementSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
    

    entities.Each<Position, Velocity, Acceleration>(
        [dt](ecs::Entity, Position& cords, Velocity& vel, Acceleration& acc) {
            if (IsEqual(cords.y_, 0.0)) {
                //!
                // vel.vx_ += acc.ax_ * dt;
            }

            vel.vy_ += acc.ay_ * dt;
        });

    entities.Each<Position, Velocity, Rotation>(
        [dt, &events](ecs::Entity entity, Position& pos, Velocity& vel, Rotation& rotation) {
            pos.y_ = std::max(0.0, pos.y_ + vel.vy_ * dt);
            pos.x_ += vel.vx_ * dt;
            // logger::Print("offset: {}\n", vel.vx_);
            if (vel.vx_ < 0) {
                rotation.is_flipped_ = true;
            } else if (vel.vx_ > 0) {
                rotation.is_flipped_ = false;
            }

            if ((IsEqual(pos.y_, 0.0)) && (!IsEqual(vel.vy_, 0.0))) {
                LandingEvent landing_event{entity};
                events.Emit<LandingEvent>(landing_event);

                if (IsEqual(vel.vx_, 0.0)) {
                    events.Emit<MovementStopEvent>();
                }
            }
        });
}

void MovementSystem::Receive(const PlayerInitiatedEvent& new_player) {
    ecs::Entity player_entity = new_player.entity_;

    player_entity.Assign<Position>();
    player_entity.Assign<Velocity>();
    player_entity.Assign<Acceleration>();
    player_entity.Assign<Rotation>();
}
