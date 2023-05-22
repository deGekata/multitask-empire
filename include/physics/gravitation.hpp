#ifndef H_GRAVITATION
#define H_GRAVITATION

#include <deque>

#include <ecs/event.hpp>
#include <ecs/system.hpp>

#include <events/player_events.hpp>
#include <events/gravitation_events.hpp>

class GravitationSystem : public ecs::System<GravitationSystem>, public ecs::Receiver<GravitationSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const PlayerCommandEvent& event);
    void Receive(const LandingEvent& event);
};

#endif
