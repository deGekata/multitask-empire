#ifndef H_MOVEMENT_COMMANDS
#define H_MOVEMENT_COMMANDS

#include <deque>

#include <ecs/event.hpp>
#include <ecs/system.hpp>

#include <events/player_events.hpp>

const int64_t kJumpSpeed = 100;
const int64_t kMoveSpeed = 50;

class MovementCommandsSystem : public ecs::System<MovementCommandsSystem>,
                               public ecs::Reciever<MovementCommandsSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Recieve(const PlayerCommandEvent& event);
private:
    std::deque<PlayerCommandEvent> events_queue_;
};

#endif
