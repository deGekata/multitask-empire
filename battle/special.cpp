#include <battle/special.hpp>

#include <components/battle_components.hpp>

#include <events/battle_events.hpp>

static constexpr double kSpecialPeriodicIncrease = 1e-05;
static constexpr double kSpecialLimit = 100.0;

void SpecialSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events_ = &events;

    events_->Subscribe<PlayerInitiatedEvent>(*this);
    events_->Subscribe<PlayerCommandEvent>(*this);
}

void SpecialSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta dt) {
    entities.Each<Special>([dt](ecs::Entity, Special& special) {
        special.experience_ = std::min(100.0, special.experience_ + dt * kSpecialPeriodicIncrease);
    });
}

void SpecialSystem::Receive(const PlayerCommandEvent& event) {
    if (event.cmd_ == PlayerCommand::SPECIAL) {
        ecs::Entity entity = event.entity_;

        auto special = entity.GetComponent<Special>();

        if (special->experience_ >= kSpecialLimit) {
            events_->Emit<SpecialTriggerEvent>(entity);

            special->experience_ = 0.0;
        }
    }
}

void SpecialSystem::Receive(const PlayerInitiatedEvent& event) {
    ecs::Entity entity = event.entity_;

    entity.Assign<Special>(Special{0.0});
}
