#ifndef H_SPECIAL
#define H_SPECIAL

#include <ecs/quick.hpp>

#include <events/player_events.hpp>

class SpecialSystem : public ecs::System<SpecialSystem>, public ecs::Reciever<SpecialSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const PlayerCommandEvent& event);
    void Receive(const PlayerInitiatedEvent& event);
};

#endif
