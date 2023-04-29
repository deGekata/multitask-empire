#ifndef H_PlAYER_EVENTS
#define H_PlAYER_EVENTS

#include <string>
#include <ecs/event.hpp>

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

#endif
