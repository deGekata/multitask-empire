#include <battle/skill_change.hpp>

#include <components/battle_components.hpp>

void SkillChangeSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    events.Subscribe<KeyPressedEvent>(*this);
    events.Subscribe<PlayerInitiatedEvent>(*this);
}

void SkillChangeSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
}

void SkillChangeSystem::Receive(const PlayerInitiatedEvent& event) {
    ecs::Entity player = event.entity_;

    if (player.HasComponent<PlayerTag>()) {
        logger::Print("SET PLAYER\n");

        player_ = player;
    }
}

void SkillChangeSystem::Receive(const KeyPressedEvent& event) {
    if (event.data_.key == igraphicslib::KeyboardKey::Tab) {
        auto ability = player_.GetComponent<SpecialAbility>();

        ability->type_ = static_cast<SpecialAbility::Type>((static_cast<int>(ability->type_) + 1) % static_cast<int>(SpecialAbility::Type::ABILITIES_AMOUNT));
    }
}
