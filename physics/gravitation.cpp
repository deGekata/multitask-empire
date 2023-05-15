#include <physics/gravitation.hpp>

#include <components/movement_components.hpp>

void GravitationSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
    events.Subscribe<LandingEvent>(*this);
}

void GravitationSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void GravitationSystem::Recieve(const PlayerCommandEvent& event) {
    if (event.cmd_ == PlayerCommand::JUMP) {
        ecs::Entity jumper = event.entity_;

        if (jumper.HasComponent<Acceleration>()) {
            jumper.GetComponent<Acceleration>()->ay_ = -kAttraction;
        }
    }
}

void GravitationSystem::Recieve(const LandingEvent& event) {
    ecs::Entity lander = event.target_;

    if (lander.HasComponent<Acceleration>()) {
        lander.GetComponent<Acceleration>()->ay_ = 0;
    }

    if (lander.HasComponent<Velocity>()) {
        auto vel = lander.GetComponent<Velocity>();

        vel->vx_ = 0;
        vel->vy_ = 0;
    }
}
