#include <physics/gravitation.hpp>

#include <components/movement_components.hpp>

static constexpr double kAttraction = 0.000000025;

void GravitationSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
    events.Subscribe<LandingEvent>(*this);
}

void GravitationSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void GravitationSystem::Receive(const PlayerCommandEvent& event) {

    ecs::Entity cmd_ent = event.cmd_;

    auto cmd_type = cmd_ent.GetComponent<PlayerCommand>().Get();
    if(cmd_type->type_ != PlayerCommandType::Action) return;

    ecs::Entity player_entity = event.entity_;
    auto action_type = cmd_ent.GetComponent<ActionCommand>().Get();

    if (action_type->type_ == ActionCommandType::Jump) {
        ecs::Entity jumper = event.entity_;

        if (jumper.HasComponent<Acceleration>()) {
            jumper.GetComponent<Acceleration>()->ay_ = -kAttraction;
        }
    }
}

void GravitationSystem::Receive(const LandingEvent& event) {
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
