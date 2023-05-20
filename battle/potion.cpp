#include <battle/potion.hpp>

#include <components/battle_components.hpp>
#include <components/collision_components.hpp>
#include <components/graphic_components.hpp>
#include <components/movement_components.hpp>

#include <events/curses_events.hpp>
#include <events/renderer_events.hpp>

static constexpr double kPotionBeginHeight = 50.0;
static constexpr double kPotionAttraction = 0.00000001;
static constexpr double kPotionVelocityX = 0.0005;
static constexpr double kPotionVelocityY = 0.003;

static constexpr double kCloudWidth = 200.0;
static constexpr double kCloudHeight = 100.0;

static constexpr double kPotionSpeedDecrease = 0.5;
static constexpr double kPotionJumpDecrease = 0.01;

static constexpr ecs::TimeDelta kSettledTime = 5000000.0;
static constexpr ecs::TimeDelta kCursedTime = 1e5;

void PotionSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    entities_ = &entities;
    events_ = &events;

    FillPotionStates();

    events_->Subscribe<CollisionEvent>(*this);
    events_->Subscribe<SpecialTriggerEvent>(*this);
    events_->Subscribe<LandingEvent>(*this);
}

void PotionSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta dt) {
    std::vector<ecs::Entity> delete_candidates;

    entities.Each<SettledPotionTag>([dt, &delete_candidates](ecs::Entity potion, SettledPotionTag& tag) {
        tag.time_left -= dt;

        if (tag.time_left <= 0) {
            delete_candidates.push_back(potion);
        }
    });

    for (auto& it : delete_candidates) {
        it.Destroy();
    }
}

void PotionSystem::Receive(const CollisionEvent& event) {
    if (event.first_collided_.HasComponent<SettledPotionTag>() ^
        event.second_collided_.HasComponent<SettledPotionTag>()) {
        ecs::Entity victim = event.first_collided_;
        ecs::Entity potion = event.second_collided_;

        if (victim.HasComponent<SettledPotionTag>()) {
            std::swap(victim, potion);
        }

        if (victim == potion.GetComponent<SettledPotionTag>()->owner) {
            return;
        }

        if (!potioned_entities_.contains(victim)) {
            potioned_entities_.insert(victim);

            auto active_potion_curse = [](ecs::Entity cursed_entity) {
                if (cursed_entity.HasComponent<BlockedTag>()) {
                    cursed_entity.Remove<BlockedTag>();
                }
            };

            events_->Emit<ActiveCurseEvent>(victim, ActiveCurseInfo{kCursedTime, std::move(active_potion_curse)});

            if (victim.HasComponent<MoveSpeed>()) {
                victim.GetComponent<MoveSpeed>()->value_ *= kPotionSpeedDecrease;
            }

            if (victim.HasComponent<JumpSpeed>()) {
                victim.GetComponent<JumpSpeed>()->value_ *= kPotionJumpDecrease;
            }

            auto passive_potion_curse = [this](ecs::Entity cursed_entity) {
                if (cursed_entity.HasComponent<MoveSpeed>()) {
                    cursed_entity.GetComponent<MoveSpeed>()->value_ /= kPotionSpeedDecrease;
                }

                if (cursed_entity.HasComponent<JumpSpeed>()) {
                    cursed_entity.GetComponent<JumpSpeed>()->value_ /= kPotionJumpDecrease;
                }

                potioned_entities_.erase(cursed_entity);
            };

            events_->Emit<PassiveCurseEvent>(victim, PassiveCurseInfo{kCursedTime, std::move(passive_potion_curse)});
        }
    }
}

void PotionSystem::Receive(const SpecialTriggerEvent& event) {
    ecs::Entity entity = event.entity_;
    if (entity.GetComponent<SpecialAbility>()->type_ == SpecialAbility::Type::Potion) {
        ProcessPotion(event.entity_);
    }
}

void PotionSystem::Receive(const LandingEvent& event) {
    if (!event.target_.HasComponent<FlyingPotionTag>()) {
        return;
    }

    ecs::Entity potion = event.target_;

    ecs::Entity owner = potion.GetComponent<FlyingPotionTag>()->owner;
    potion.Remove<FlyingPotionTag>();
    potion.Assign<SettledPotionTag>(SettledPotionTag{owner, kSettledTime});

    potion.Assign<HitBox>(HitBox{kCloudWidth, kCloudHeight});

    events_->Emit<SpriteSheetStateChangedEvent>(MissleStates::SETTLED, potion);
}

void PotionSystem::ProcessPotion(ecs::Entity thrower) {
    ecs::Entity potion = entities_->Create();

    auto thrower_position = thrower.GetComponent<Position>();
    auto thrower_box = thrower.GetComponent<HitBox>();
    auto thrower_rotation = thrower.GetComponent<Rotation>();

    Position potion_position = *thrower_position;
    Velocity potion_velocity = {-kPotionVelocityX, kPotionVelocityY};
    Acceleration potion_acceleration = {0, -kPotionAttraction};

    potion_position.y_ += kPotionBeginHeight;
    if (!thrower_rotation->is_flipped_) {
        potion_position.x_ += thrower_box->width_;
        potion_velocity.vx_ *= -1;
    } else {
        potion_position.x_ -= thrower_box->width_;
    }

    potion.AssignFromCopy<Rotation>(*thrower_rotation);
    potion.AssignFromCopy<Position>(potion_position);
    potion.AssignFromCopy<Velocity>(potion_velocity);
    potion.AssignFromCopy<Acceleration>(potion_acceleration);

    potion.Assign<FlyingPotionTag>(FlyingPotionTag{thrower});

    potion.Assign<RenderFrameData>(RenderFrameData{0, false});
    events_->Emit<SkinChangeRequest>(state_name_converter_, MissleStates::FLYING, "./assets/sprites/potion.png",
                                     potion);
}

void PotionSystem::FillPotionStates() {
    state_name_converter_["SETTLED"] = MissleStates::SETTLED;
    state_name_converter_["FLYING"] = MissleStates::FLYING;
}
