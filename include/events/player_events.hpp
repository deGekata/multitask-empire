#ifndef H_PlAYER_EVENTS
#define H_PlAYER_EVENTS

#include <string>
#include <sstream>

#include <ecs/event.hpp>

#include <components/player_components.hpp>

enum class PlayerCommand {
    INVALID = -1,
    IDLE,
    WALK_LEFT,
    WALK_RIGHT,
    ATTACK_ONE,
    ATTACK_TWO,
    DEATH,
    JUMP,
    TEXT_INSERT_REQUEST
};

struct PlayerInitiatedEvent : public ecs::Event<PlayerInitiatedEvent> {
    explicit PlayerInitiatedEvent(ecs::Entity player_entity) : entity_(player_entity) {
    }

    PlayerInitiatedEvent(const PlayerInitiatedEvent& other) = default;
    ~PlayerInitiatedEvent() override = default;

    ecs::Entity entity_;
};

struct PlayerCommandEvent : public ecs::Event<PlayerCommandEvent> {
    explicit PlayerCommandEvent(PlayerCommand cmd, ecs::Entity player_entity) : cmd_(cmd), entity_(player_entity) {
    }

    PlayerCommandEvent(const PlayerCommandEvent& other) = default;
    ~PlayerCommandEvent() override = default;

    PlayerCommand cmd_;
    ecs::Entity entity_;
};

struct PlayerTextRequestEvent : public ecs::Event<PlayerTextRequestEvent> {
    explicit PlayerTextRequestEvent(std::stringstream ss)
        : cmd_(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>()) {
    }

    PlayerTextRequestEvent(const PlayerTextRequestEvent& other) = default;
    ~PlayerTextRequestEvent() override = default;

    std::vector<std::string> cmd_;
};

struct SpriteSheetLoadRequest : public ecs::Event<SpriteSheetLoadRequest> {

    explicit SpriteSheetLoadRequest(const std::string& xml_path) : xml_path_(xml_path) {
    }

    SpriteSheetLoadRequest(const SpriteSheetLoadRequest& other) = default;
    ~SpriteSheetLoadRequest() override = default;

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
    explicit SkinChangeRequest(std::string skin_name, ecs::Entity entity) : skin_name_(skin_name), entity_(entity) {
    }

    SkinChangeRequest(const SkinChangeRequest& other) = default;
    ~SkinChangeRequest() override = default;

    std::string skin_name_;
    ecs::Entity entity_;
};

#endif
