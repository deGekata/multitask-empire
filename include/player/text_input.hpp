#ifndef H_TEXT_INPUT
#define H_TEXT_INPUT

#include <deque>
#include <string>

#include <ecs/system.hpp>
#include <ecs/event.hpp>

#include <events/player_events.hpp>

class TextInputSystem : public ecs::System<TextInputSystem>, public ecs::Reciever<TextInputSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const PlayerCommandEvent& event);
private:
    // std::deque<std::string> cmds_;
    ecs::EventManager*     events_;
};

#endif
