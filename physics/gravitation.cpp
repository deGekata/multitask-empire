#include <physics/gravitation.hpp>

#include <components/movement_components.hpp>

void GravitationSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
    events.Subscribe<LandingEvent>(*this);
}

void GravitationSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
    for (auto jumper = jumpers_queue_.front(); !jumpers_queue_.empty(); jumpers_queue_.pop_front()) {
        auto acceleration = jumper.GetComponent<Acceleration>();

        acceleration->ay_ = -kAttraction;
    }

    for (auto lander = landers_queue_.front(); !landers_queue_.empty(); landers_queue_.pop_front()) {
        auto acceleration = lander.GetComponent<Acceleration>();
        auto velocity = lander.GetComponent<Velocity>();

        acceleration->ay_ = 0;
        velocity->vy_ = 0;
        velocity->vx_ = 0;
    }
}

void GravitationSystem::Recieve(const PlayerCommandEvent& event) {
    if (event.cmd_ == PlayerCommand::JUMP) {
        jumpers_queue_.push_back(event.entity_);
    }
}

void GravitationSystem::Recieve(const LandingEvent& event) {
    landers_queue_.push_back(event.target_);
}
