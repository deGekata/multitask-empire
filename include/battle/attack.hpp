#ifndef H_ATTACK
#define H_ATTACK

#include <unordered_map>
#include <queue>

#include <components/battle_components.hpp>

#include <events/player_events.hpp>

#include <ecs/quick.hpp>

static constexpr uint32_t kBasicHealth = 100;
static constexpr uint32_t kBasicAttackPower = 10;

static constexpr double kBasicAttackSpeed = 5;
static constexpr double kBasicAttackDistance = 20;

class AttackSystem : public ecs::System<AttackSystem>, public ecs::Reciever<AttackSystem> {
    struct AttackFrame {
        ecs::Entity entity_;

        AttackSpeed speed_;
        AttackDistance distance_;
    };

public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const PlayerInitiatedEvent& event);
    void Receive(const PlayerCommandEvent& event);

private:
    void ProcessAttackers(ecs::EntityManager& entities);
    void UpdateFrames(ecs::TimeDelta dt);

    std::queue<ecs::Entity> attackers_queue_;
    std::unordered_map<ecs::Entity, AttackFrame> attack_frame_map_;
};

#endif
