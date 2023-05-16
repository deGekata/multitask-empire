#ifndef H_PLAYER
#define H_PLAYER

#include <iostream>
#include <string>
#include <map>

#include <ecs/system.hpp>

#include <events/player_events.hpp>

void FillPlayerStatesNameMap(std::map<std::string, int>* storage);

class PlayerSystem : public ecs::System<PlayerSystem>, public ecs::Reciever<PlayerSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const PlayerCommandEvent& event);
    void Receive(const PlayerTextRequestEvent& event);
private:
    int changed_state_;
    ecs::Entity player_;
    std::map<std::string, int> state_name_to_player_state_id_;
    ecs::EventManager* event_manager_;
};

#endif  
