#include <spritesheet/spritesheet.hpp>

void SpriteSheetSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    // LoadSpriteSheet("default_player_skin.png", "default_player_xml.png");
    events.Subscribe<SpriteSheetLoadRequest, SpriteSheetSystem>(*this);

    entities_ = &entities;
    ecs::Entity spritesheet_storage = entities.Create();
    spritesheet_storage.Assign<SpriteSheetStorageTag>();
}

void SpriteSheetSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void SpriteSheetSystem::LoadSpriteSheet(const std::string& xml_path) {

    entities_->Each<SpriteSheetStorageTag>([this, &xml_path](ecs::Entity entity, SpriteSheetStorageTag&){
        parser_.Parse(xml_path, entity);
    });
}

void SpriteSheetSystem::Recieve(const SpriteSheetLoadRequest& event) {
    LoadSpriteSheet(event.xml_path_);
}
