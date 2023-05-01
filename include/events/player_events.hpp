#ifndef H_PlAYER_EVENTS
#define H_PlAYER_EVENTS

#include <string>
#include <ecs/event.hpp>

#include <components/player_components.hpp>

//? to review
enum class PLAYER_CMD {
    INVALID = -1,
    IDLE,
    WALK_LEFT,
    WALK_RIGHT,
    ATTACK_ONE,
    ATTACK_TWO,
    GET_DOWN,
    JUMP,
};
const size_t MAX_N_CMDS = 200;

struct PlayerInitiatedEvent : public ecs::Event<PlayerInitiatedEvent> {
    explicit PlayerInitiatedEvent(ecs::Entity player_entity): entity_(player_entity) {
    }

    PlayerInitiatedEvent(const PlayerInitiatedEvent& other) = default;

    ~PlayerInitiatedEvent() override{
    }

    ecs::Entity entity_;
};

struct PlayerCommandEvent : public ecs::Event<PlayerCommandEvent> {
    explicit PlayerCommandEvent(PLAYER_CMD cmd, ecs::Entity player_entity): cmd_(cmd), entity_(player_entity) {
    }

    PlayerCommandEvent(const PlayerCommandEvent& other) = default;

    ~PlayerCommandEvent() override{
    }

    PLAYER_CMD  cmd_;
    ecs::Entity entity_;
};

struct SpriteSheetLoadRequest : public ecs::Event<SpriteSheetLoadRequest> {

    explicit SpriteSheetLoadRequest(const std::string& xml_path): xml_path_(xml_path){
    }

    SpriteSheetLoadRequest(const SpriteSheetLoadRequest& other) = default;

    ~SpriteSheetLoadRequest() override{
    }

    std::string xml_path_;
};

// todo: to ObjectStateChanged
//? change with help of tracker
// struct PlayerStateChanged : public ecs::Event<PlayerStateChanged> {

//     explicit PlayerStateChanged(PLAYER_STATE state): new_state_(state){
//     }

//     ~PlayerStateChanged() override{
//     }

//     PLAYER_STATE new_state_;
// };

struct SkinChangeRequest : public ecs::Event<SkinChangeRequest> {
    explicit SkinChangeRequest(std::string skin_name, ecs::Entity entity): skin_name_(skin_name), entity_(entity){
    }

    SkinChangeRequest(const SkinChangeRequest& other) = default;
    ~SkinChangeRequest() override{
    }

    std::string skin_name_;
    ecs::Entity entity_;
};

#endif
