#ifndef H_MOVEMENT_COMMANDS
#define H_MOVEMENT_COMMANDS

#include <deque>

#include <ecs/event.hpp>
#include <ecs/system.hpp>

#include <events/player_events.hpp>

static constexpr double kDefaultJumpSpeed = 0.005;
static constexpr double kDefaultMoveSpeed = 0.0005;

class MovementCommandsSystem : public ecs::System<MovementCommandsSystem>,
                               public ecs::Reciever<MovementCommandsSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const PlayerCommandEvent& event);
    void Receive(const PlayerInitiatedEvent& event);
};

#endif
