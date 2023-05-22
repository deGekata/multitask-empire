#ifndef H_SPECIAL
#define H_SPECIAL

#include <ecs/quick.hpp>

#include <events/player_events.hpp>
#include <events/action_events.hpp>

class SpecialSystem : public ecs::System<SpecialSystem>, public ecs::Receiver<SpecialSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const ActionCommandEvent& event);
    void Receive(const PlayerInitiatedEvent& event);

private:
    ecs::EventManager* events_;
};

#endif
