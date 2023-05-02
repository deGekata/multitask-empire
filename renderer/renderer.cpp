#include <renderer/renderer.hpp>

#include <iostream>

#include <components/movement_components.hpp>

void RendererSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PendingMovementEvent>(*this);
    events.Subscribe<LandingEvent>(*this);
    events.Subscribe<MovementStopEvent>(*this);
}

void RendererSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
    for (auto target : inspected_entities_) {
        if (target.HasComponent<Position>()) {
            auto position = target.GetComponent<Position>();

            logger::Print(kInfo, "{} has coordinates {} {}\n", target.GetId().GetIndex(), position->x_, position->y_);
        }
    }
}

void RendererSystem::Recieve(const PendingMovementEvent& event) {
    inspected_entities_.insert(event.target_);
}

void RendererSystem::Recieve(const LandingEvent& event) {
    logger::Print(kInfo, "{} landed\n", event.target_.GetId().GetIndex());
}

void RendererSystem::Recieve(const MovementStopEvent& event) {
    inspected_entities_.erase(event.target_);

    logger::Print(kInfo, "{} stopped moving\n", event.target_.GetId().GetIndex());
}
