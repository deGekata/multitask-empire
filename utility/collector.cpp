#include <utility/collector.hpp>

#include <components/movement_components.hpp>

static constexpr double kBigBigConstant = 1e6;

void GarbageCollectorSystem::Configure(ecs::EntityManager&, ecs::EventManager&) {
}

void GarbageCollectorSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta) {
    entities.Each<Position>([](ecs::Entity entity, Position& pos) {
        if ((std::abs(pos.x_) + std::abs(pos.y_)) >= kBigBigConstant) {
            entity.Destroy();
        }
    });
}
