#ifndef H_SLIME
#define H_SLIME

#include <queue>

#include <components/movement_components.hpp>

#include <ecs/quick.hpp>

#include <events/battle_events.hpp>
#include <events/collision_events.hpp>

struct FlyingSlimeTag {};

struct AttachedSlimeTag {
    ecs::TimeDelta time_left;
};

class SlimeSystem : public ecs::System<SlimeSystem>, public ecs::Reciever<SlimeSystem> {
    enum SlimeStates : int { FLYING, ATTACHED };

public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const SpecialTriggerEvent& event);
    void Receive(const CollisionEvent& event);

private:
    void ProcessSlime(ecs::Entity owner_entity);
    void UpdateAttached(ecs::EntityManager& entities, ecs::TimeDelta dt);

    static Position GetAttachedPosition(Position owner_position);

    std::map<std::string, int> state_name_converter_;

    ecs::EntityManager* entities_;
    ecs::EventManager* events_;
};

#endif
