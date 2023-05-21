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

// todo: replace
struct BattleAbleConfigChangeRequest : public ecs::Event<BattleAbleConfigChangeRequest> {
    explicit BattleAbleConfigChangeRequest(std::string name, ecs::Entity entity) : 
        config_path_(name), entity_(entity) {
    }

    BattleAbleConfigChangeRequest(const BattleAbleConfigChangeRequest& other) = default;
    ~BattleAbleConfigChangeRequest() override = default;

    std::string                 config_path_;
    ecs::Entity                 entity_;
};

struct BattleAbleConfigLoadRequest : public ecs::Event<BattleAbleConfigLoadRequest> {

    explicit BattleAbleConfigLoadRequest(const std::string& config_path) : config_path_(config_path) {
    }

    BattleAbleConfigLoadRequest(const BattleAbleConfigLoadRequest& other) = default;
    ~BattleAbleConfigLoadRequest() override = default;

    std::string config_path_;
};

struct SpriteSheetStateChangedEvent : public ecs::Event<SpriteSheetStateChangedEvent> {
	explicit SpriteSheetStateChangedEvent(int state_id, ecs::Entity entity, bool is_one_shot = false) : state_id_(state_id), entity_(entity), is_one_shot_(is_one_shot) {
    }

    int         state_id_;
    ecs::Entity entity_;
    bool        is_one_shot_;
};

struct StateRenderedEvent : public ecs::Event<StateRenderedEvent> {
	explicit StateRenderedEvent(ecs::Entity entity) : entity_(entity) {
    }

    ecs::Entity entity_;
};

#endif
