#include <battle/special.hpp>

void SpecialSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    events.Subscribe<PlayerInitiatedEvent>(*this);
    events.Subscribe<PlayerCommandEvent>(*this);
}

void SpecialSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {

}
