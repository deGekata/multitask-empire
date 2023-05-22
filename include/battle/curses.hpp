#ifndef H_CURSES
#define H_CURSES

#include <ecs/quick.hpp>

#include <events/curses_events.hpp>
#include <events/player_events.hpp>

class CursesSystem : public ecs::System<CursesSystem>, public ecs::Receiver<CursesSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const PlayerInitiatedEvent& event);
    void Receive(const ActiveCurseEvent& event);
    void Receive(const PassiveCurseEvent& event);
};

#endif
