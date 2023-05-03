#ifndef SPRITESHEET_PARSER_H
#define SPRITESHEET_PARSER_H

#include <components/graphic_components.hpp>
#include <ecs/system.hpp>
#include <events/player_events.hpp>

#include <spritesheet/xml_parser.hpp>

class SpriteSheetSystem : public ecs::System<SpriteSheetSystem>, public ecs::Reciever<SpriteSheetSystem>{
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    // parses xml file at path xml_path, creates component Spritesheet
    void LoadSpriteSheet(const std::string& xml_path);

    void Recieve(const SpriteSheetLoadRequest& event);
    void Recieve(const PlayerTextRequestEvent& event);
    void Recieve(const SkinChangeRequest& event);
    
private:
    void ChangeSkin(ecs::Entity entity, std::string skin_path);
private:
    // todo: remove, cause system shouldn't storage any data
    XmlParser parser_;
    ecs::EntityManager* entities_;
};

#endif // SPRITESHEET_PARSER_H
