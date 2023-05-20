#include <objects_configuration/battleable_configuration.hpp>

#include <filesystem>

void BattleAbleObjectsConfigSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<BattleAbleConfigChangeRequest, BattleAbleObjectsConfigSystem>(*this);
    events.Subscribe<BattleAbleConfigLoadRequest, BattleAbleObjectsConfigSystem>(*this);
}

void BattleAbleObjectsConfigSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    
    if(!load_requests_.empty()){
        for(auto req = load_requests_.front(); !load_requests_.empty(); req = load_requests_.front()){
            LoadConfig(req.config_path_, entities);
        }
    }

    if(requests_.empty()) return;

    for(auto req = requests_.front(); !requests_.empty(); req = requests_.front()){
        bool is_changed = false;

        entities.Each<BattleAbleAttributesStorageTag, BattleAbleAttributes>(
        [&req, &is_changed](ecs::Entity, BattleAbleAttributesStorageTag&, BattleAbleAttributes& attrs) {

            if (attrs.config_path_ == req.config_path_) {
                is_changed = true;
            }
        });

        if(!is_changed) {
            LoadConfig(req.config_path_, entities);
        }

        // todo: refactor
        entities.Each<BattleAbleAttributesStorageTag, BattleAbleAttributes>(
        [this, &req, &events](ecs::Entity, BattleAbleAttributesStorageTag&, BattleAbleAttributes& attrs) {

            if (attrs.config_path_ == req.config_path_) {
                ChangeConfiguration(req.entity_, attrs, events);
            }
        });

        requests_.pop();
    }
}

void BattleAbleObjectsConfigSystem::Receive(const BattleAbleConfigChangeRequest& event) {
    requests_.push(event);
}

void BattleAbleObjectsConfigSystem::Receive(const BattleAbleConfigLoadRequest& event) {
    load_requests_.push(event);
}

void BattleAbleObjectsConfigSystem::LoadConfig(const std::string& path, ecs::EntityManager& entities) {
    parser_.Parse(path, &entities);
}

void BattleAbleObjectsConfigSystem::ChangeConfiguration(ecs::Entity entity, BattleAbleAttributes& attrs, ecs::EventManager& events) {

    if (entity.HasComponent<BattleAbleAttributes>()) {
        entity.Replace<BattleAbleAttributes>(attrs);
    } else {
        entity.Assign<BattleAbleAttributes>(attrs);
    }

    std::map<std::string, int> state_name_to_id_;
    state_name_to_id_[attrs.basic_frames_.run_] = static_cast<int>(BattleAbleState::Run);
    state_name_to_id_[attrs.basic_frames_.idle_] = static_cast<int>(BattleAbleState::Idle);
    state_name_to_id_[attrs.basic_frames_.jump_] = static_cast<int>(BattleAbleState::Jump);
    state_name_to_id_[attrs.basic_frames_.death_] = static_cast<int>(BattleAbleState::Death);

    for(uint n_attack = 0; n_attack < attrs.attacks_.size(); n_attack++) {
        state_name_to_id_[attrs.attacks_[n_attack].xml_name_] = static_cast<int>(BattleAbleState::Attack) + n_attack;
    }
    
    events.Emit<SkinChangeRequest>(state_name_to_id_, static_cast<int>(BattleAbleState::Idle), attrs.sprite_sheet_path_, entity);
    
    PBattleAbleAttributes p_attrs;
    p_attrs.attr_ = &attrs;

    entity.AssignFromCopy<PBattleAbleAttributes>(p_attrs);
}
