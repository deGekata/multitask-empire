#include <battle/attack.hpp>

#include <components/collision_components.hpp>
#include <components/movement_components.hpp>
#include <components/utility_components.hpp>

static constexpr double kBasicHealth = 100.0;
static constexpr double kBasicAttackPower = 10.0;

static constexpr double kBasicAttackSpeed = 1.0;
static constexpr double kBasicAttackDistance = 50;

void AttackSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    events.Subscribe<PlayerInitiatedEvent>(*this);
    events.Subscribe<PlayerCommandEvent>(*this);
}

void AttackSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta dt) {
    ProcessAttackers(entities);
    UpdateFrames(dt);
}

void AttackSystem::ProcessAttackers(ecs::EntityManager& entities) {
    while (!attackers_queue_.empty()) {
        auto attacker = attackers_queue_.front();
        attackers_queue_.pop();

        ecs::Entity attack_frame;

        if (attack_frame_map_.find(attacker) == attack_frame_map_.end()) {
            attack_frame = entities.Create();

            attack_frame.Assign<DamagerTag>(DamagerTag{attacker});
            attack_frame.Assign<Position>();
            attack_frame.Assign<HitBox>();
            attack_frame.Assign<AttackPower>();

            auto attach_strategy = [attack_frame, attacker](Position pos) mutable {
                auto rotation = attacker.GetComponent<Rotation>();

                auto attacker_box = attacker.GetComponent<HitBox>();
                auto frame = attack_frame.GetComponent<HitBox>();

                Position new_position;
                new_position.y_ = pos.y_;
                if (rotation->is_flipped_) {
                    new_position.x_ = pos.x_ - frame->width_;
                } else {
                    new_position.x_ = pos.x_ + attacker_box->width_;
                }

                return new_position;
            };

            attack_frame.Assign<Attached>(Attached{attacker, std::move(attach_strategy)});

            auto speed = attacker.GetComponent<AttackSpeed>();
            auto distance = attacker.GetComponent<AttackDistance>();

            attack_frame_map_.insert(std::make_pair(attacker, AttackFrame{attack_frame, *speed, *distance}));
        } else {
            attack_frame = attack_frame_map_[attacker].entity_;
        }

        auto power = attack_frame.GetComponent<AttackPower>();
        power->power_ = attacker.GetComponent<AttackPower>()->power_;

        auto collide_frame = attack_frame.GetComponent<HitBox>();

        collide_frame->height_ = attacker.GetComponent<HitBox>()->height_;
        collide_frame->width_ = 0;
    }
}

void AttackSystem::UpdateFrames(ecs::TimeDelta dt) {
    std::vector<ecs::Entity> delete_candidates;

    for (auto it = attack_frame_map_.begin(); it != attack_frame_map_.end(); it++) {
        ecs::Entity attacker = it->first;

        auto attack_frame = it->second.entity_.GetComponent<HitBox>();
        attack_frame->width_ += it->second.speed_.speed_ * dt;

        if (attack_frame->width_ > it->second.distance_.distance_) {
            it->second.entity_.Destroy();
            delete_candidates.push_back(attacker);

            continue;
        }
    }

    for (auto& to_delete : delete_candidates) {
        attack_frame_map_.erase(to_delete);
    }
}

void AttackSystem::Receive(const PlayerInitiatedEvent& event) {
    ecs::Entity entity = event.entity_;

    entity.Assign<Health>(Health{kBasicHealth});
    entity.Assign<AttackSpeed>(AttackSpeed{kBasicAttackSpeed});
    entity.Assign<AttackDistance>(AttackDistance{kBasicAttackDistance});
    entity.Assign<AttackPower>(AttackPower{kBasicAttackPower});
}

void AttackSystem::Receive(const PlayerCommandEvent& event) {
    ecs::Entity cmd_ent = event.cmd_;
    auto cmd_type = cmd_ent.GetComponent<PlayerCommand>().Get();

    if (cmd_type->type_ == PlayerCommandType::Action) {
        auto action_type = cmd_ent.GetComponent<ActionCommand>();

        if(action_type->type_ == ActionCommandType::Attack)
            attackers_queue_.push(event.entity_);
    }
}
