#ifndef H_BLOCK
#define H_BLOCK

#include <ecs/quick.hpp>

#include <events/player_events.hpp>
#include <events/action_events.hpp>

const ecs::TimeDelta kDefaultShieldDuration = 1000;

class BlockSystem : public ecs::System<BlockSystem>, public ecs::Receiver<BlockSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const ActionCommandEvent& event);
    void Receive(const PlayerInitiatedEvent& event);
};

#endif
