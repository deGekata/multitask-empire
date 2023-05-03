#ifndef H_MOVEMENT
#define H_MOVEMENT

#include <utility/metric.hpp>

#include <ecs/system.hpp>

#include <events/player_events.hpp>
class MovementSystem : public ecs::System<MovementSystem>, public ecs::Reciever<MovementSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Recieve(const PlayerInitiatedEvent& new_player);
private:
    metrics::TimeStorage timestamp_;
};

#endif
