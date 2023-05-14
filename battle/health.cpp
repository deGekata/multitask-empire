#include <battle/health.hpp>

#include <components/battle_components.hpp>

void HealthSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<CollisionEvent>(*this);
}

void HealthSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta) {
    entities.Each<Health>([](ecs::Entity entity, Health& health) {
        if (health.health_ < 0) {
            entity.Destroy();
        }
    });
}

void HealthSystem::Recieve(const CollisionEvent& event) {
    if ((event.first_collided_.HasComponent<DamagerTag>()) || (event.second_collided_.HasComponent<DamagerTag>())) {
        auto damaged_entity = event.second_collided_;
        auto damager_entity = event.first_collided_;

        if (event.second_collided_.HasComponent<DamagerTag>()) {
            std::swap(damager_entity, damaged_entity);
        }

        if (damager_entity.GetComponent<DamagerTag>()->creator_ == damaged_entity) {
            return;
        }

        if (damaged_entity.HasComponent<Health>()) {
            auto health = damaged_entity.GetComponent<Health>();
            health->health_ -= damager_entity.GetComponent<AttackPower>()->power_;

            logger::Print("{} now has health {}\n", damaged_entity.GetId().GetIndex(), health->health_);
        }
    }
}
