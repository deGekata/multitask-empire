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

            std::cout << "[*] " << target.GetId().GetIndex() << " has coordinates (" << position->x_ << "; "
                      << position->y_ << ")" << std::endl;
        }
    }
}

void RendererSystem::Recieve(const PendingMovementEvent& event) {
    inspected_entities_.insert(event.target_);
}

void RendererSystem::Recieve(const LandingEvent& event) {
    std::cout << "[*] " << event.target_.GetId().GetIndex() << " landed" << std::endl;
}

void RendererSystem::Recieve(const MovementStopEvent& event) {
    inspected_entities_.erase(event.target_);

    std::cout << "[*] " << event.target_.GetId().GetIndex() << " stopped moving" << std::endl;
}
