#ifndef H_DISPATCHER
#define H_DISPATCHER

#include <deque>

#include <ecs/system.hpp>

#include <events/player_events.hpp>

class DispatcherSystem : public ecs::System<DispatcherSystem>, public ecs::Reciever<DispatcherSystem> {
public:
    DispatcherSystem(bool& running_flag);

    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Recieve(const PlayerCommandEvent& cmd);

private:
    bool& running_;

    std::deque<PlayerCommandEvent> events_queue_;
};

#endif
