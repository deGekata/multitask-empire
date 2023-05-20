#include <battle/slime.hpp>

#include <components/battle_components.hpp>
#include <components/collision_components.hpp>
#include <components/graphic_components.hpp>
#include <components/utility_components.hpp>

#include <events/curses_events.hpp>
#include <events/renderer_events.hpp>

static constexpr double kBasicMissleWidth = 50.0;
static constexpr double kBasicMissleHeight = 50.0;
static constexpr double kBasicMissleSpeed = 0.0005;

static constexpr ecs::TimeDelta kBasicSlimeHoldTime = 5000000.0;

static constexpr double kBasicDamageDecrease = 0.5;
static constexpr double kBasicSpeedDecrease = 0.1;

void SlimeSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    entities_ = &entities;
    events_ = &events;

    events.Subscribe<CollisionEvent>(*this);
    events.Subscribe<SpecialTriggerEvent>(*this);

    state_name_converter_["FLYING"] = SlimeStates::FLYING;
    state_name_converter_["ATTACHED"] = SlimeStates::ATTACHED;
}

void SlimeSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
    UpdateAttached(entities, dt);
}

void SlimeSystem::ProcessSlime(ecs::Entity owner_entity) {
    ecs::Entity slime = entities_->Create();

    auto firing_position = owner_entity.GetComponent<Position>();
    auto firing_box = owner_entity.GetComponent<HitBox>();
    auto firing_rotation = owner_entity.GetComponent<Rotation>();

    Position missle_position = *firing_position;
    Velocity missle_velocity = {-kBasicMissleSpeed, 0};

    missle_position.y_ += kBasicMissleHeight;
    if (!firing_rotation->is_flipped_) {
        missle_position.x_ += firing_box->width_;
        missle_velocity.vx_ *= -1;
    } else {
        missle_position.x_ -= firing_box->width_;
    }

    slime.AssignFromCopy<Rotation>(*firing_rotation);
    slime.AssignFromCopy<Velocity>(missle_velocity);
    slime.AssignFromCopy<Position>(missle_position);

    slime.Assign<HitBox>(kBasicMissleWidth, kBasicMissleHeight);
    slime.Assign<FlyingSlimeTag>();

    slime.Assign<RenderFrameData>(RenderFrameData{0, false});
    events_->Emit<SkinChangeRequest>(state_name_converter_, SlimeStates::FLYING, "./assets/sprites/slime.png", slime);
}

void SlimeSystem::UpdateAttached(ecs::EntityManager& entities, ecs::TimeDelta dt) {
    std::vector<ecs::Entity> slimes_to_destroy;

    entities.Each<AttachedSlimeTag>([dt, &slimes_to_destroy](ecs::Entity slime, AttachedSlimeTag& attached) {
        attached.time_left -= dt;
        if (attached.time_left <= 0) {
            slimes_to_destroy.push_back(slime);
        }
    });

    for (auto& slime : slimes_to_destroy) {
        slime.Destroy();
    }
}

void SlimeSystem::Receive(const SpecialTriggerEvent& event) {
    ecs::Entity entity = event.entity_;
    if (entity.GetComponent<SpecialAbility>()->type_ == SpecialAbility::Type::Slime) {
        ProcessSlime(event.entity_);
    }
}

void SlimeSystem::Receive(const CollisionEvent& event) {
    if (event.first_collided_.HasComponent<FlyingSlimeTag>() || event.second_collided_.HasComponent<FlyingSlimeTag>()) {
        ecs::Entity slime_entity = event.first_collided_;
        ecs::Entity owner_entity = event.second_collided_;

        if (event.second_collided_.HasComponent<FlyingSlimeTag>()) {
            std::swap(slime_entity, owner_entity);
        }

        if (slime_entity.HasComponent<Velocity>()) {
            slime_entity.GetComponent<Velocity>()->vx_ = 0;
        }

        slime_entity.Remove<FlyingSlimeTag>();

        slime_entity.Assign<AttachedSlimeTag>(AttachedSlimeTag{kBasicSlimeHoldTime});
        slime_entity.Assign<Attached>(Attached{owner_entity, &GetAttachedPosition});

        owner_entity.GetComponent<MoveSpeed>()->value_ *= kBasicSpeedDecrease;
        auto speed_recovery = [](ecs::Entity entity) {
            entity.GetComponent<MoveSpeed>()->value_ /= kBasicSpeedDecrease;
        };

        owner_entity.GetComponent<AttackPower>()->power_ *= kBasicDamageDecrease;
        auto power_recovery = [](ecs::Entity entity) {
            entity.GetComponent<AttackPower>()->power_ /= kBasicDamageDecrease;
        };

        events_->Emit<SpriteSheetStateChangedEvent>(SlimeStates::ATTACHED, slime_entity);

        events_->Emit<PassiveCurseEvent>(owner_entity,
                                         PassiveCurseInfo{kBasicSlimeHoldTime, std::move(speed_recovery)});
        events_->Emit<PassiveCurseEvent>(owner_entity,
                                         PassiveCurseInfo{kBasicSlimeHoldTime, std::move(power_recovery)});
    }
}

Position SlimeSystem::GetAttachedPosition(Position owner_position) {
    return owner_position;
}
