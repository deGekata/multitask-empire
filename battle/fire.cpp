#include <battle/fire.hpp>

#include <components/battle_components.hpp>
#include <components/movement_components.hpp>
#include <components/collision_components.hpp>
#include <components/graphic_components.hpp>

#include <events/renderer_events.hpp>

static constexpr double kBasicMissleWidth = 50.0;
static constexpr double kBasicMissleHeight = 50.0;
static constexpr double kBasicMissleSpeed = 0.0005;

static constexpr double kBasicFireballMultiplier = 0.01;

void FireSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<SpecialTriggerEvent>(*this);

    state_name_converter_["FIRE"] = MissleStates::FLYING;
}

void FireSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    ProcessFires(entities, events);
}

void FireSystem::ProcessFires(ecs::EntityManager& entities, ecs::EventManager& events) {
    while (!fires_queue_.empty()) {
        ecs::Entity firing_entity = fires_queue_.front();
        fires_queue_.pop();

        ecs::Entity missle = entities.Create();

        auto firing_position = firing_entity.GetComponent<Position>();
        auto firing_box = firing_entity.GetComponent<HitBox>();
        auto firing_rotation = firing_entity.GetComponent<Rotation>();

        Position missle_position = *firing_position;
        Velocity missle_velocity = {-kBasicMissleSpeed, 0.0};

        missle_position.y_ += kBasicMissleHeight;
        if (!firing_rotation->is_flipped_) {
            missle_position.x_ += firing_box->width_;
            missle_velocity.vx_ *= -1;
        } else {
            missle_position.x_ -= firing_box->width_;
        }

        missle.AssignFromCopy<Rotation>(*firing_rotation);
        missle.AssignFromCopy<Velocity>(missle_velocity);
        missle.AssignFromCopy<Position>(missle_position);

        missle.Assign<HitBox>(kBasicMissleWidth, kBasicMissleHeight);

        missle.AssignFromCopy<DamagerTag>(DamagerTag{firing_entity});
        missle.AssignFromCopy<AttackPower>(
            AttackPower{kBasicFireballMultiplier * firing_entity.GetComponent<AttackPower>()->power_});

        events.Emit<SkinChangeRequest>(state_name_converter_, MissleStates::FLYING, "./assets/sprites/fireball.png",
                                       missle);
    }
}

void FireSystem::Receive(const SpecialTriggerEvent& event) {
    ecs::Entity entity = event.entity_;
    if (entity.GetComponent<SpecialAbility>()->type_ == SpecialAbility::Type::Fireball) {
        fires_queue_.push(event.entity_);
    }
}
