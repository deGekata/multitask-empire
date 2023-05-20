#include <objects_configuration/spritesheet.hpp>

#include <filesystem>

static constexpr auto kSpritesPath = "./assets/sprites/";

void SpriteSheetSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    // LoadSpriteSheet("default_player_skin.png", "default_player_xml.png");
    events.Subscribe<SpriteSheetLoadRequest, SpriteSheetSystem>(*this);
    events.Subscribe<SkinChangeRequest, SpriteSheetSystem>(*this);

    entities_ = &entities;
    // todo: remove
    LoadSprites();
}

void SpriteSheetSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void SpriteSheetSystem::LoadSpriteSheet(const std::string& xml_path) {
    parser_.Parse(xml_path, entities_);
}

void SpriteSheetSystem::Receive(const SpriteSheetLoadRequest& event) {
    LoadSpriteSheet(event.xml_path_);
}

void SpriteSheetSystem::Receive(const SkinChangeRequest& event) {
    ChangeSkin(event.entity_, event.skin_name_, event.state_name_to_id_, event.init_state_);
}

void SpriteSheetSystem::ChangeSkin(ecs::Entity entity_to_skin, const std::string& skin_path, const std::map<std::string, int>& state_name_to_id, int init_state) {

    SpriteSheet* sprite_sheet = nullptr;

    entities_->Each<SpriteSheetStorageTag, SpriteSheet>(
    [&sprite_sheet, &skin_path](ecs::Entity entity, SpriteSheetStorageTag&,SpriteSheet& stored_sprite_sheet) {

        if (stored_sprite_sheet.img_path_ == skin_path) {
            sprite_sheet = entity.GetComponent<SpriteSheet>().Get();
        }
    });

    if(sprite_sheet == nullptr) {
        logger::Print(kWarning, "unable to find requested sprite sheet: {}\n", skin_path);
        return;
    }

    ObjectAnimationData animation_data;
    animation_data.cur_frame_                     = 0;
    animation_data.n_sprite_sheet_state_          = static_cast<uint>(init_state);
    animation_data.sprite_sheet_.sprite_sheet_    = sprite_sheet;

    const auto& states_storage = animation_data.sprite_sheet_.sprite_sheet_->states_;
    for(uint n_state = 0; n_state < states_storage.size(); n_state++) {
        if(state_name_to_id.count(states_storage[n_state].name_) == 0) {
            continue; //logger::Print(kWarning, );
        }

        int id = state_name_to_id.at(states_storage[n_state].name_);
        if(animation_data.sprite_sheet_.id_to_n_state_mapping_.count(id) == 0){
            //?
            continue;
        }
        animation_data.sprite_sheet_.id_to_n_state_mapping_[id] = n_state;
    }   

    if (entity_to_skin.HasComponent<ObjectAnimationData>()) {
        entity_to_skin.Replace<ObjectAnimationData>(std::move(animation_data));
    } else {
        entity_to_skin.Assign<ObjectAnimationData>(animation_data);
    }
}

void SpriteSheetSystem::LoadSprites() {
    for (auto& file_name : std::filesystem::directory_iterator(kSpritesPath)) {
        if (file_name.path().extension() == ".xml") {
            LoadSpriteSheet(file_name.path());
        }  
    }
}
