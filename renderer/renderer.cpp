#include <renderer/renderer.hpp>
#include <thread>
#include <chrono>
#include <limits>

#include <components/movement_components.hpp>
#include <components/graphic_components.hpp>
#include <components/player_components.hpp>

#include <events/graphic_events.hpp>
#include <cassert>
static constexpr double kSecondsOnRerender = 0.04;
static constexpr double kSecondsOnFrameChange = 0.16;

// todo: enchance, when normal states for objects would be implemented
static std::string PLAYER_CMD_TO_STR[MAX_N_CMDS] = {"INVALID"};

#define ADD_CMD_STR_MATCH(cmd) PLAYER_CMD_TO_STR[static_cast<uint>(PLAYER_CMD::cmd)] = #cmd;

RendererSystem::RendererSystem() :
    window_(1000, 800, "Simple"),
    cur_state_(PLAYER_CMD::INVALID)
{
    ADD_CMD_STR_MATCH(IDLE)
    ADD_CMD_STR_MATCH(WALK_LEFT)
    ADD_CMD_STR_MATCH(WALK_RIGHT)
    ADD_CMD_STR_MATCH(ATTACK_ONE)
    ADD_CMD_STR_MATCH(ATTACK_TWO)
    ADD_CMD_STR_MATCH(GET_DOWN)
    ADD_CMD_STR_MATCH(JUMP)
}

void RendererSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    // events.Subscribe<PendingMovementEvent>(*this);
    // events.Subscribe<LandingEvent>(*this);
    // events.Subscribe<MovementStopEvent>(*this);
    events.Subscribe<PlayerInitiatedEvent>(*this);
    events.Subscribe<PlayerCommandEvent>(*this);
    window_.Show();

    events.Emit<WindowInitiatedEvent>(&window_);

    render_init_time_  = std::chrono::high_resolution_clock::now();
    state_change_init_time_ = std::chrono::high_resolution_clock::now();
}

void RendererSystem::LaunchAnimationFrame(const ObjectAnimationData& animation_data, const Position& cur_pos) {

    typename SpriteSheet::StateFrame frame_pos = animation_data.sprite_sheet_->states_[animation_data.n_sprite_sheet_state_].positions_[animation_data.cur_frame_];
    
    igraphicslib::Rect rect(frame_pos.x_, frame_pos.y_, frame_pos.w_, frame_pos.h_);
    animation_data.sprite_sheet_->sprite_.SetTexture(animation_data.sprite_sheet_->texture_);
    auto tmp = animation_data.sprite_sheet_->sprite_.Crop(rect);

    int x_coord = static_cast<int>(cur_pos.x_) - static_cast<int>(frame_pos.x_offset_);
    int y_coord = static_cast<int>(window_.GetSurfRect().h) - static_cast<int>(frame_pos.h_) - (static_cast<int>(cur_pos.y_));

    window_.DrawSprite({x_coord, y_coord}, tmp);
}

void RendererSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
    
    std::chrono::high_resolution_clock::time_point cur_count = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration<double>(cur_count - render_init_time_);

    if(duration.count() < kSecondsOnRerender) {
        return;
    }

    bool is_frame_change = false;
    duration = std::chrono::duration<double>(cur_count - state_change_init_time_);

    if(duration.count() >= kSecondsOnFrameChange) {
        is_frame_change = true;
        state_change_init_time_ = std::chrono::high_resolution_clock::now();
    }

    render_init_time_ = std::chrono::high_resolution_clock::now();

    window_.Clear();

    for (auto target : inspected_entities_) {

        if(target.HasComponent<PlayerTag>() && target.HasComponent<ObjectAnimationData>()) {            
            Position cur_pos = *target.GetComponent<Position>();
            ObjectAnimationData* player_animation_data = target.GetComponent<ObjectAnimationData>().Get();
            assert(player_animation_data);

            LaunchAnimationFrame(*player_animation_data, cur_pos);

            if(is_frame_change) {

                if(player_animation_data->cur_frame_ + 1 == player_animation_data->sprite_sheet_->states_[player_animation_data->n_sprite_sheet_state_].positions_.size()) {
                    player_animation_data->cur_frame_ = 0;
                }
                else player_animation_data->cur_frame_++;
            }
        }
    }

    window_.Update();
    // std::this_thread::sleep_for(std::chrono::milliseconds(ANIMATION_FREEZE_TIME));
}

// void RendererSystem::Recieve(const PendingMovementEvent& event) {
//     inspected_entities_.insert(event.target_);
// }

void RendererSystem::Recieve(const PlayerCommandEvent& event) {
    
    // todo: implement something like playerStateDispatcherSystem
    if((event.cmd_ != PLAYER_CMD::WALK_LEFT)  && (event.cmd_ != PLAYER_CMD::WALK_RIGHT) && (event.cmd_ != PLAYER_CMD::ATTACK_ONE) &&
       (event.cmd_ != PLAYER_CMD::ATTACK_TWO) && (event.cmd_ != PLAYER_CMD::GET_DOWN)   && (event.cmd_ != PLAYER_CMD::IDLE)     && 
       (event.cmd_ != PLAYER_CMD::JUMP) && (event.cmd_ != PLAYER_CMD::INVALID)) {

        return;
    }

    if(event.cmd_ == cur_state_){
        return;
    }

    if(event.cmd_ == PLAYER_CMD::IDLE) {
        cur_state_ = PLAYER_CMD::INVALID;
    }
    else {
        cur_state_ = event.cmd_;
    }

    for(auto target : inspected_entities_){
        if(!target.HasComponent<PlayerTag>() || !target.HasComponent<ObjectAnimationData>()){
            continue;
        }

        ObjectAnimationData* animation_storage = target.GetComponent<ObjectAnimationData>().Get();

        if(animation_storage->sprite_sheet_ == nullptr) {
            std::cout << "player doesn't have attached spritesheet, so new state won't appear on da screan\n";
            return;
        }

        std::string str_cmd = PLAYER_CMD_TO_STR[static_cast<uint>(event.cmd_)];
        uint n_state_in_sprite_sheet = UINT32_MAX;
        
        size_t n_states_in_spritesheet = animation_storage->sprite_sheet_->states_.size();
        std::vector<typename SpriteSheet::StateData>* sprite_sheet_states = &animation_storage->sprite_sheet_->states_;

        for(uint n_state = 0; n_state < n_states_in_spritesheet; n_state++) {
            if(sprite_sheet_states->at(n_state).name_ == str_cmd) {
                n_state_in_sprite_sheet = n_state;
                break;
            }
        }

        if(n_state_in_sprite_sheet == UINT32_MAX) {
            std::cout << "state data not found on xml " << animation_storage->sprite_sheet_->img_path_ << ", exiting\n";
            assert(0);
        } 

        animation_storage->n_sprite_sheet_state_ = n_state_in_sprite_sheet;
        animation_storage->cur_frame_            = 0;
    }
}

// void RendererSystem::Recieve(const LandingEvent& event) {
//     logger::Print(kInfo, "{} landed\n", event.target_.GetId().GetIndex());
// }

// void RendererSystem::Recieve(const MovementStopEvent& event) {
//     inspected_entities_.erase(event.target_);

//     logger::Print(kInfo, "{} stopped moving\n", event.target_.GetId().GetIndex());
// }

void RendererSystem::Recieve(const PlayerInitiatedEvent& event) {
    inspected_entities_.insert(event.entity_);
}

// void RendererSystem::Recieve(const LandingEvent& event) {
//     log(INFO, "{} landed\n", event.target_.GetId().GetIndex());
// }

// void RendererSystem::Recieve(const MovementStopEvent& event) {
//     inspected_entities_.erase(event.target_);

//     log(INFO, "{} stopped moving\n", event.target_.GetId().GetIndex());
// }
