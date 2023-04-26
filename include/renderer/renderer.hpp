#ifndef H_RENDERER
#define H_RENDERER

#include <unordered_set>

#include <ecs/system.hpp>

#include <events/movement_events.hpp>
#include <events/gravitation_events.hpp>

class RendererSystem : public ecs::System<RendererSystem>, public ecs::Reciever<RendererSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Recieve(const PendingMovementEvent& cmd);
    void Recieve(const LandingEvent& cmd);
    void Recieve(const MovementStopEvent& cmd);

private:
    std::unordered_set<ecs::Entity> inspected_entities_;
};

#endif
