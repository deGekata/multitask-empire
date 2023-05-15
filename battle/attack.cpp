#include <battle/attack.hpp>

#include <components/collision_components.hpp>
#include <components/movement_components.hpp>

void AttackSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerInitiatedEvent>(*this);
    events.Subscribe<PlayerCommandEvent>(*this);
}

void AttackSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta dt) {
    ProcessAttackers(entities);
    UpdateFrames(dt);
}

void AttackSystem::ProcessAttackers(ecs::EntityManager& entities) {
    if (attackers_queue_.empty()) {
        return;
    }
    
    for (auto attacker = attackers_queue_.front(); !attackers_queue_.empty(); attackers_queue_.pop()) {
        ecs::Entity attack_frame;

        if (attack_frame_map_.find(attacker) == attack_frame_map_.end()) {
            attack_frame = entities.Create();

            attack_frame.Assign<DamagerTag>(DamagerTag{attacker});
            attack_frame.Assign<Position>();
            attack_frame.Assign<HitBox>();
            attack_frame.Assign<AttackPower>();

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
        auto position = attacker.GetComponent<Position>();
        auto rotation = attacker.GetComponent<Rotation>();
        auto frame = attacker.GetComponent<HitBox>();

        auto attack_frame = it->second.entity_.GetComponent<HitBox>();
        attack_frame->width_ += it->second.speed_.speed_ * dt;

        if (attack_frame->width_ > it->second.distance_.distance_) {
            it->second.entity_.Destroy();
            delete_candidates.push_back(attacker);

            continue;
        }

        auto attack_position = it->second.entity_.GetComponent<Position>();
        attack_position->y_ = position->y_;
        if (rotation->is_flipped_) {
            attack_position->x_ = position->x_ - frame->width_;
        } else {
            attack_position->x_ = position->x_ + frame->width_;
        }
    }

    for (auto& to_delete : delete_candidates) {
        attack_frame_map_.erase(to_delete);
    }
}

void AttackSystem::Recieve(const PlayerInitiatedEvent& event) {
    ecs::Entity entity = event.entity_;

    entity.Assign<Health>(Health{kBasicHealth});
    entity.Assign<AttackSpeed>(AttackSpeed{kBasicAttackSpeed});
    entity.Assign<AttackDistance>(AttackDistance{kBasicAttackDistance});
    entity.Assign<AttackPower>(AttackPower{kBasicAttackPower});
}

void AttackSystem::Recieve(const PlayerCommandEvent& event) {
    if ((event.cmd_ == PlayerCommand::ATTACK_ONE) || (event.cmd_ == PlayerCommand::ATTACK_TWO)) {
        attackers_queue_.push(event.entity_);
    }
}
