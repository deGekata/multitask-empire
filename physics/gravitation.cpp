#include <physics/gravitation.hpp>

#include <components/movement_components.hpp>

void GravitationSystem::Update(ecs::EntityManager& entities, ecs::TimeDelta) {
    entities.Each<Position, Acceleration>([](ecs::Entity, Position& pos, Acceleration& acc) {
        // TODO Change to recieve of Jump event
        if (pos.y_ > 0) {
          acc.ay_ = -kAttraction;
        }
        // TODO Move to recieve of Landing event
        else {
          acc.ay_ = 0;
        }
    });
}
