#include <utility/attach.hpp>

#include <components/utility_components.hpp>

void AttachSystem::Configure(ecs::EntityManager&, ecs::EventManager&) {
}

void AttachSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta) {
    entities.Each<Attached, Position>([](ecs::Entity, Attached& attached, Position& pos) {
        auto owner_pos = attached.owner.GetComponent<Position>();

        pos = attached.pos_modifier(*owner_pos);
    });
}
