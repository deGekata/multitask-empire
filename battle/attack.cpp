#include <battle/attack.hpp>

#include <components/collision_components.hpp>
#include <components/movement_components.hpp>
#include <components/utility_components.hpp>

void AttackSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
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
                auto frame = attack_frame.GetComponent<HitBox>();

                Position new_position;
                new_position.y_ = pos.y_;
                if (rotation->is_flipped_) {
                    new_position.x_ = pos.x_ - frame->width_;
                } else {
                    new_position.x_ = pos.x_ + frame->width_;
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
        power->power_multiplier = attacker.GetComponent<AttackPower>()->power_multiplier;

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
        attack_frame->width_ += static_cast<int64_t>(kBasicAttackSpeed * it->second.speed_.speed_multiplier) * dt;

        if (attack_frame->width_ >
            static_cast<int64_t>(kBasicAttackDistance * it->second.distance_.distance_multiplier)) {
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
    entity.Assign<AttackSpeed>(AttackSpeed{});
    entity.Assign<AttackDistance>(AttackDistance{});
    entity.Assign<AttackPower>(AttackPower{});
}

void AttackSystem::Receive(const PlayerCommandEvent& event) {
    if ((event.cmd_ == PlayerCommand::ATTACK_ONE) || (event.cmd_ == PlayerCommand::ATTACK_TWO)) {
        attackers_queue_.push(event.entity_);
    }
}
