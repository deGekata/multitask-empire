#ifndef H_MOVEMENT_COMMANDS
#define H_MOVEMENT_COMMANDS

#include <deque>

#include <ecs/event.hpp>
#include <ecs/system.hpp>

#include <events/player_events.hpp>
#include <events/action_events.hpp>

class MovementCommandsSystem : public ecs::System<MovementCommandsSystem>,
                               public ecs::Reciever<MovementCommandsSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const ActionCommandEvent& event);
    void Receive(const PlayerInitiatedEvent& event);
};

#endif
