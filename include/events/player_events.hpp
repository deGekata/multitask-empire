#ifndef H_PlAYER_EVENTS
#define H_PlAYER_EVENTS

#include <string>
#include <ecs/event.hpp>

#include <components/player_components.hpp>

struct PlayerCommandEvent {
    std::string command_;
};

struct SpriteSheetLoadRequest : public ecs::Event<SpriteSheetLoadRequest> {

    explicit SpriteSheetLoadRequest(const std::string& xml_path): xml_path_(xml_path){
    }
    virtual ~SpriteSheetLoadRequest() override{
    }

    std::string xml_path_;
};

// todo: to ObjectStateChanged
//? change with help of tracker
struct PlayerStateChanged : public ecs::Event<PlayerStateChanged> {

    explicit PlayerStateChanged(PLAYER_STATE state): new_state_(state){
    }

    virtual ~PlayerStateChanged() override{
    }

    PLAYER_STATE new_state_;
};

struct SkinChangeRequest : public ecs::Event<SkinChangeRequest> {
    explicit SkinChangeRequest(std::string skin_name, ecs::Entity entity): skin_name_(skin_name), entity_(entity){
    }

    virtual ~SkinChangeRequest() override{
    }

    std::string skin_name_;
    ecs::Entity entity_;
};

#endif
