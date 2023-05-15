#ifndef H_GRAPHICS_EVENTS
#define H_GRAPHICS_EVENTS

#include <string>
#include <sstream>

#include <ecs/entity.hpp>
#include <ecs/event.hpp>



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
