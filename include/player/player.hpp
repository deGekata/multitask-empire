#ifndef H_PLAYER
#define H_PLAYER

#include <iostream>
#include <string>
#include <map>
#include <deque>

#include <ecs/system.hpp>

#include <events/player_events.hpp>


class PlayerSystem : public ecs::System<PlayerSystem>, public ecs::Reciever<PlayerSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const KeyPressedEvent& event);
    void Receive(const KeyReleasedEvent& event);

    void Receive(const PlayerInitiatedEvent& event);
    void Receive(const PlayerTextRequestEvent& event);
private:
    // int changed_state_;
    ecs::Entity player_;
    std::map<std::string, int> state_name_to_player_state_id_;
    ecs::EventManager* event_manager_;

    // Map which gives command by taking keyboard key
    std::map<igraphicslib::KeyboardKey, PlayerCommandType> key_to_cmd_matcher_;   
    std::map<igraphicslib::KeyboardKey, ActionCommandType> key_to_action_matcher_;
    
    std::deque<igraphicslib::KeyEventData> commands_queue_;
    std::deque<igraphicslib::KeyEventData> on_release_queue_;
};

#endif  
