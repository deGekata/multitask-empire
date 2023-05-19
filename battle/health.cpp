#include <battle/attack.hpp>
#include <battle/health.hpp>

#include <components/battle_components.hpp>

void HealthSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events_ = &events;

    events_->Subscribe<CollisionEvent>(*this);
    events_->Subscribe<DamageTakenEvent>(*this);
}

void HealthSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta) {
    entities.Each<Health>([](ecs::Entity entity, Health& health) {
        if (health.health_ < 0) {
            entity.Destroy();
        }
    });
}

void HealthSystem::Receive(const CollisionEvent& event) {
    if ((event.first_collided_.HasComponent<DamagerTag>()) || (event.second_collided_.HasComponent<DamagerTag>())) {
        auto damaged_entity = event.second_collided_;
        auto damager_entity = event.first_collided_;

        if (event.second_collided_.HasComponent<DamagerTag>()) {
            std::swap(damager_entity, damaged_entity);
        }

        if (damager_entity.GetComponent<DamagerTag>()->creator_ == damaged_entity) {
            return;
        }

        events_->Emit<DamageTakenEvent>(damaged_entity, damager_entity.GetComponent<AttackPower>()->power_);
    }
}

void HealthSystem::Receive(const DamageTakenEvent& event) {
    ecs::Entity damaged_entity = event.damaged_entity_;

    if (damaged_entity.HasComponent<BlockedTag>()) {
        return;
    }

    if (damaged_entity.HasComponent<Health>()) {
        auto health = damaged_entity.GetComponent<Health>();
        health->health_ -= event.damage_amount_;

        logger::Print("{} now has health {}\n", damaged_entity.GetId().GetIndex(), health->health_);
    }
}
