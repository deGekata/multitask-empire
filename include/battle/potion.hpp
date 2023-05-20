#ifndef H_POTION
#define H_POTION

#include <map>
#include <set>

#include <ecs/quick.hpp>

#include <events/battle_events.hpp>
#include <events/collision_events.hpp>
#include <events/gravitation_events.hpp>

struct FlyingPotionTag {
    ecs::Entity owner;
};

struct SettledPotionTag {
    ecs::Entity owner;

    ecs::TimeDelta time_left;
};

class PotionSystem : public ecs::System<PotionSystem>, public ecs::Reciever<PotionSystem> {
    enum MissleStates : int { SETTLED, FLYING };

public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const CollisionEvent& event);
    void Receive(const SpecialTriggerEvent& event);
    void Receive(const LandingEvent& event);

private:
    void FillPotionStates();

    void ProcessPotion(ecs::Entity thrower);

    std::map<std::string, int> state_name_converter_;
    std::set<ecs::Entity> potioned_entities_;

    ecs::EntityManager* entities_;
    ecs::EventManager* events_;
};

#endif
