#include <battle/slime.hpp>

#include <components/battle_components.hpp>
#include <components/movement_components.hpp>
#include <components/collision_components.hpp>
#include <components/graphic_components.hpp>

#include <events/renderer_events.hpp>

static const uint32_t kBasicMissleWidth = 50;
static const uint32_t kBasicMissleHeight = 50;
static const int64_t kBasicMissleSpeed = 20;

static const ecs::TimeDelta kBasicSlimeHoldTime = 1000000;

static const double kBasicDamageMultiplier = 0.1;
static const double kBasicSpeedDecrease = 0.5;

void SlimeSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    events.Subscribe<CollisionEvent>(*this);
    events.Subscribe<PlayerCommandEvent>(*this);

    state_name_converter_["FLYING"] = SlimeStates::FLYING;
    state_name_converter_["ATTACHED"] = SlimeStates::ATTACHED;
}

void SlimeSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
    ProcessSlimes(entities, events);
    ProcessAttach(entities, events);
    UpdateAttached(entities, dt);
}

void SlimeSystem::ProcessSlimes(ecs::EntityManager& entities, ecs::EventManager& events) {
    if (slime_queue_.empty()) {
        return;
    }

    for (auto slimer_entity = slime_queue_.front(); !slime_queue_.empty(); slime_queue_.pop()) {
        ecs::Entity slime = entities.Create();

        auto firing_position = slimer_entity.GetComponent<Position>();
        auto firing_box = slimer_entity.GetComponent<HitBox>();
        auto firing_rotation = slimer_entity.GetComponent<Rotation>();

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

        slime.Assign<RenderFrameData>(RenderFrameData{0, true});
        events.Emit<SkinChangeRequest>(state_name_converter_, SlimeStates::FLYING, "./assets/sprites/fireball.png",
                                       slime);
    }
}

void SlimeSystem::ProcessAttach(ecs::EntityManager& entities, ecs::EventManager& events) {
    while (!state_change_queue_.empty()) {
        ecs::Entity slime_to_change = state_change_queue_.front();
        state_change_queue_.pop();

        events.Emit<SpriteSheetStateChangedEvent>(SlimeStates::FLYING, slime_to_change);
    }
}

void SlimeSystem::UpdateAttached(ecs::EntityManager& entities, ecs::TimeDelta dt) {
    std::vector<ecs::Entity> slimes_to_destroy;

    entities.Each<AttachedSlimeTag, Position>(
        [dt, &slimes_to_destroy](ecs::Entity slime, AttachedSlimeTag& attached, Position& pos) {
            pos = *attached.owner.GetComponent<Position>();

            attached.time_left -= dt;
            if (attached.time_left <= 0) {
                slimes_to_destroy.push_back(slime);
            }
        });

    for (auto& slime : slimes_to_destroy) {
        slime.Destroy();
    }
}

void SlimeSystem::Receive(const PlayerCommandEvent& event) {
    if (event.cmd_ == PlayerCommand::SPECIAL) {
        ecs::Entity entity = event.entity_;
        if (entity.GetComponent<SpecialAbility>()->type_ == SpecialAbility::Type::Slime) {
            slime_queue_.push(event.entity_);
        }
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
        slime_entity.Assign<AttachedSlimeTag>(AttachedSlimeTag{owner_entity, kBasicSlimeHoldTime});

        state_change_queue_.push(slime_entity);
    }
}
