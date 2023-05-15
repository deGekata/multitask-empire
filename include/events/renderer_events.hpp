#ifndef H_GRAPHICS_EVENTS
#define H_GRAPHICS_EVENTS

#include <string>
#include <sstream>

#include <ecs/entity.hpp>
#include <ecs/event.hpp>

// todo: rename skin -> spritesheet

struct SpriteSheetLoadRequest : public ecs::Event<SpriteSheetLoadRequest> {

    explicit SpriteSheetLoadRequest(const std::string& xml_path) : xml_path_(xml_path) {
    }

    SpriteSheetLoadRequest(const SpriteSheetLoadRequest& other) = default;
    ~SpriteSheetLoadRequest() override = default;

    std::string xml_path_;
};

struct SkinChangeRequest : public ecs::Event<SkinChangeRequest> {
    explicit SkinChangeRequest(const std::map<std::string, int>& state_name_to_id, int init_state, std::string skin_name, ecs::Entity entity) : 
        state_name_to_id_(state_name_to_id), init_state_(init_state), skin_name_(skin_name), entity_(entity) {
    }

    SkinChangeRequest(const SkinChangeRequest& other) = default;
    ~SkinChangeRequest() override = default;

    std::map<std::string, int>  state_name_to_id_;
    int                         init_state_;
    std::string                 skin_name_;
    ecs::Entity                 entity_;
};

struct SpriteSheetStateChangedEvent : public ecs::Event<SpriteSheetStateChangedEvent> {
	explicit SpriteSheetStateChangedEvent(int state_id, ecs::Entity entity) : state_id_(state_id), entity_(entity){
    }

    int         state_id_;
    ecs::Entity entity_;
};

#endif
