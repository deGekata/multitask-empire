#include <renderer/renderer.hpp>

#include <iostream>
#include <thread>
#include <chrono>

#include <components/movement_components.hpp>
#include <components/graphic_components.hpp>
#include <components/player_components.hpp>

const int64_t ANIMATION_FREEZE_TIME = 100; //ms
// todo: enchance
std::string player_state_to_str(PLAYER_STATE state) {

    switch(state) {
        case PLAYER_STATE::DEFAULT:
            return "IDLE";
        case PLAYER_STATE::DOWN:
            return "DOWN";
        case PLAYER_STATE::JUMP:
            return "JUMP";
        case PLAYER_STATE::LOWER_ATTACK:
            return "ATTACK_DOWN";
        case PLAYER_STATE::UPPER_ATTACK:
            return "ATTACK_UP";
    }
}

RendererSystem::RendererSystem() :
    window_(1000, 800, "Simple"){}

void RendererSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    events.Subscribe<PendingMovementEvent>(*this);
    events.Subscribe<LandingEvent>(*this);
    events.Subscribe<MovementStopEvent>(*this);
    events.Subscribe<PlayerStateChanged>(*this);
    window_.Show();

    inspected_entities_.insert(*entities.GetEntitiesWithComponents<PlayerTag>().begin());
}

void RendererSystem::LaunchAnimationFrame(const ObjectAnimationData& animation_data, const Position& cur_pos) {

    typename SpriteSheet::StateFrame frame_pos = animation_data.sprite_sheet_->states_[animation_data.n_sprite_sheet_state_].positions_[animation_data.cur_frame_];
    
    igraphicslib::Rect rect(frame_pos.x_, frame_pos.y_, frame_pos.w_, frame_pos.h_);
    animation_data.sprite_sheet_->sprite_.SetTexture(animation_data.sprite_sheet_->texture_);
    auto tmp = animation_data.sprite_sheet_->sprite_.Crop(rect);
    
    window_.DrawSprite({cur_pos.x_, cur_pos.y_}, tmp);
}

void RendererSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
    window_.Clear();

    for (auto target : inspected_entities_) {
        if(target.HasComponent<PlayerTag>() && target.HasComponent<ObjectAnimationData>()) {            
            Position cur_pos = *target.GetComponent<Position>();
            ObjectAnimationData* player_animation_data = target.GetComponent<ObjectAnimationData>().Get();
            assert(player_animation_data);

            LaunchAnimationFrame(*player_animation_data, cur_pos);

            if(player_animation_data->cur_frame_ + 1 == player_animation_data->sprite_sheet_->states_[player_animation_data->n_sprite_sheet_state_].positions_.size()) {
                player_animation_data->cur_frame_ = 0;
            }
            else player_animation_data->cur_frame_++;
        }
    }

    window_.Update();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void RendererSystem::Recieve(const PendingMovementEvent& event) {
    inspected_entities_.insert(event.target_);
}

void RendererSystem::Recieve(const PlayerStateChanged& event) {
    fmt::print("hey\n");
    for(auto target : inspected_entities_){
        if(!target.HasComponent<PlayerTag>() || !target.HasComponent<ObjectAnimationData>()){
            continue;
        }

        ObjectAnimationData* animation_storage = target.GetComponent<ObjectAnimationData>().Get();

        if(animation_storage->sprite_sheet_ == nullptr) {
            std::cout << "player doesn't have attached spritesheet, so new state won't appear on da screan\n";
            return;
        }

        std::string str_state = player_state_to_str(event.new_state_);

        int n_state_in_sprite_sheet = -1;

        size_t n_states_in_spritesheet = animation_storage->sprite_sheet_->states_.size();
        std::vector<typename SpriteSheet::StateData>* sprite_sheet_states = &animation_storage->sprite_sheet_->states_;

        for(uint n_state = 0; n_state < n_states_in_spritesheet; n_state++) {
            if(sprite_sheet_states->at(n_state).name_ == str_state) {
                n_state_in_sprite_sheet = n_state;
                break;
            }
        }

        if(n_state_in_sprite_sheet == -1) {
            std::cout << "state data not found on xml " << animation_storage->sprite_sheet_->img_path_ << ", exiting\n";
            assert(0);
        } 

        animation_storage->n_sprite_sheet_state_ = n_state_in_sprite_sheet;
    }
}

void RendererSystem::Recieve(const LandingEvent& event) {
    log(INFO, "{} landed\n", event.target_.GetId().GetIndex());
}

void RendererSystem::Recieve(const MovementStopEvent& event) {
    inspected_entities_.erase(event.target_);

    log(INFO, "{} stopped moving\n", event.target_.GetId().GetIndex());
}
