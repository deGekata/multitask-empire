#ifndef H_RENDERER
#define H_RENDERER

#include <unordered_set>
#include <queue>
#include <map>
#include <mutex>

#include <utility/metric.hpp>

#include <ecs/system.hpp>
#include <ecs/entity.hpp>

#include <events/movement_events.hpp>
#include <events/gravitation_events.hpp>
#include <events/player_events.hpp>
#include <events/renderer_events.hpp>

#include <graphics/aWindow.hpp>
#include <graphics/event.hpp>

#include <components/graphic_components.hpp>
#include <components/movement_components.hpp>

class RendererSystem : public ecs::System<RendererSystem>, public ecs::Receiver<RendererSystem> {
public:
    RendererSystem();

    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    // void Receive(const PendingMovementEvent& cmd);
    // void Receive(const LandingEvent& cmd);
    // void Receive(const MovementStopEvent& cmd);
    void Receive(const PlayerCommandEvent& event);
    void Receive(const PlayerInitiatedEvent& event);
    void Receive(const SpriteSheetStateChangedEvent& event);

private:
    void LaunchAnimationFrame(const ObjectAnimationData& animation_data, const Position& cur_pos, bool is_flipped);

private:    
    // todo: to entities??
    igraphicslib::Window            window_;

    metrics::TimeStorage rerender_timestamp_;
    metrics::TimeStorage sprite_frame_change_timestamp_;

    PlayerCommand cur_player_state_; // todo: change

    std::map<PlayerCommand, std::string> command_converter_;
};

#endif
