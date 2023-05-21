#ifndef H_BATTLE_EVENTS
#define H_BATTLE_EVENTS

#include <ecs/event.hpp>
#include <ecs/entity.hpp>

#include <components/battle_components.hpp>

struct DamageTakenEvent : public ecs::Event<DamageTakenEvent> {
    DamageTakenEvent(ecs::Entity damaged, double damage_amount)
        : damaged_entity_(damaged), damage_amount_(damage_amount) {
    }

    ecs::Entity damaged_entity_;
    double damage_amount_;
};

struct SpecialTriggerEvent : public ecs::Event<SpecialTriggerEvent> {
    explicit SpecialTriggerEvent(ecs::Entity entity) : entity_(entity) {
    }

    ecs::Entity entity_;
};

#endif
