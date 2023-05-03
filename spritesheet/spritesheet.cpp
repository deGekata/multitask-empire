#include <spritesheet/spritesheet.hpp>

void SpriteSheetSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    // LoadSpriteSheet("default_player_skin.png", "default_player_xml.png");
    events.Subscribe<SpriteSheetLoadRequest, SpriteSheetSystem>(*this);
    events.Subscribe<SkinChangeRequest, SpriteSheetSystem>(*this);

    entities_ = &entities;
    ecs::Entity spritesheet_storage = entities.Create();
    spritesheet_storage.Assign<SpriteSheetStorageTag>();

    // todo: remove
    LoadSpriteSheet("orc_berserk.xml");
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

void SpriteSheetSystem::Recieve(const SkinChangeRequest& event) {

    bool was_found = false;

    entities_->Each<SpriteSheetStorageTag, SpriteSheet>([this, &event, &was_found](ecs::Entity entity, SpriteSheetStorageTag&, SpriteSheet& sprite_sheet) {
        if(sprite_sheet.img_path_ == event.skin_name_) {
            ObjectAnimationData animation_data = {
                .sprite_sheet_ = entity.GetComponent<SpriteSheet>().Get(),
                .n_sprite_sheet_state_ = 0,
                .cur_frame_    = 0 // todo;
            };
            this->entities_->Assign<ObjectAnimationData>(event.entity_.GetId(), animation_data);
            was_found = true;
        }
    });

    if(!was_found) {
        std::cout << "Warning, unable to find requested sprite sheet: " << event.skin_name_ << "\n";
        return;
    }
}
