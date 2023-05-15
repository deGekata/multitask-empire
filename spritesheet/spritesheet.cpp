#include <spritesheet/spritesheet.hpp>

#include <filesystem>

static constexpr auto kSpritesPath = "./assets/sprites/";

static constexpr auto kSpriteAdd = "sprite";
static constexpr auto kSpriteLoad = "load_sprite";

void SpriteSheetSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    // LoadSpriteSheet("default_player_skin.png", "default_player_xml.png");
    events.Subscribe<SpriteSheetLoadRequest, SpriteSheetSystem>(*this);
    events.Subscribe<SkinChangeRequest, SpriteSheetSystem>(*this);
    events.Subscribe<PlayerTextRequestEvent, SpriteSheetSystem>(*this);

    entities_ = &entities;
    // todo: remove
    LoadSprites();
}

void SpriteSheetSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void SpriteSheetSystem::LoadSpriteSheet(const std::string& xml_path) {
    parser_.Parse(xml_path, entities_);
}

void SpriteSheetSystem::Recieve(const SpriteSheetLoadRequest& event) {
    LoadSpriteSheet(event.xml_path_);
}

void SpriteSheetSystem::Recieve(const PlayerTextRequestEvent& event) {
    if (event.cmd_.empty()) {
        return;
    }

    if (event.cmd_[0] != kSpriteAdd && event.cmd_[0] != kSpriteLoad) {
        return;
    }

    if (event.cmd_.size() == 1) {
        logger::Print(kWarning, "Specify what skin do you wanna load after command\n");
        return;
    }

    // todo: refactor
    if (event.cmd_[0] == kSpriteAdd) {
        ecs::Entity player_entity = *entities_->GetEntitiesWithComponents<PlayerTag>().begin();
        ChangeSkin(player_entity, event.cmd_[1]);
    } else if (event.cmd_[0] == kSpriteLoad) {
        LoadSpriteSheet(event.cmd_[1]);
    }
}

void SpriteSheetSystem::Recieve(const SkinChangeRequest& event) {
    ChangeSkin(event.entity_, event.skin_name_);
}

void SpriteSheetSystem::ChangeSkin(ecs::Entity entity_to_skin, std::string skin_path) {
    bool was_found = false;

    entities_->Each<SpriteSheetStorageTag, SpriteSheet>(
        [&was_found, &skin_path, &entity_to_skin](ecs::Entity entity, SpriteSheetStorageTag&,
                                                  SpriteSheet& sprite_sheet) {
            if (sprite_sheet.img_path_ == skin_path) {
                ObjectAnimationData animation_data = {
                    .sprite_sheet_ = entity.GetComponent<SpriteSheet>().Get(),
                    .n_sprite_sheet_state_ = 0,
                    .cur_frame_ = 0  // todo;
                };
                
                if (entity_to_skin.HasComponent<ObjectAnimationData>()) {
                    entity_to_skin.Replace<ObjectAnimationData>(std::move(animation_data));
                } else {
                    entity_to_skin.Assign<ObjectAnimationData>(animation_data);
                }
                was_found = true;
            }
        });

    if (!was_found) {
        logger::Print(kWarning, "unable to find requested sprite sheet: {}\n", skin_path);
        return;
    }
}

void SpriteSheetSystem::LoadSprites() {
    for (auto& file_name : std::filesystem::directory_iterator(kSpritesPath)) {
        if (file_name.path().extension() == ".xml") {
            LoadSpriteSheet(file_name.path());
        }  
    }
}
