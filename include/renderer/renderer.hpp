#ifndef H_RENDERER
#define H_RENDERER

#include <unordered_set>
#include <queue>
#include <mutex>

#include <ecs/system.hpp>
#include <ecs/entity.hpp>

#include <events/movement_events.hpp>
#include <events/gravitation_events.hpp>
#include <events/player_events.hpp>

#include <graphics/aWindow.hpp>
#include <graphics/event.hpp>

#include <components/graphic_components.hpp>
#include <components/movement_components.hpp>

class RendererSystem : public ecs::System<RendererSystem>, public ecs::Reciever<RendererSystem> {
public:
    RendererSystem();

    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    // void Recieve(const PendingMovementEvent& cmd);
    // void Recieve(const LandingEvent& cmd);
    // void Recieve(const MovementStopEvent& cmd);
    void Recieve(const PlayerCommandEvent& event);
    void Recieve(const PlayerInitiatedEvent& event);

private:
    void LaunchAnimationFrame(const ObjectAnimationData& animation_data, const Position& cur_pos);

private:
    std::unordered_set<ecs::Entity> inspected_entities_;
    // todo: to entities??
    igraphicslib::Window            window_;
};

#endif
