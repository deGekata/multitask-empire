#include <renderer/renderer.hpp>

#include <algorithm>
#include <thread>
#include <limits>

#include <components/movement_components.hpp>
#include <components/graphic_components.hpp>
#include <components/player_components.hpp>

#include <events/graphic_events.hpp>
#include <cassert>

static constexpr double kSpriteScale = 3.0;

#define ADD_CMD_STR_MATCH(cmd) command_converter_[PlayerCommand::cmd] = #cmd;

RendererSystem::RendererSystem() : window_(1000, 800, "Simple"), cur_player_state_(PlayerCommand::INVALID) {
    ADD_CMD_STR_MATCH(IDLE)
    ADD_CMD_STR_MATCH(WALK_LEFT)
    ADD_CMD_STR_MATCH(WALK_RIGHT)
    ADD_CMD_STR_MATCH(ATTACK_ONE)
    ADD_CMD_STR_MATCH(ATTACK_TWO)
    ADD_CMD_STR_MATCH(DEATH)
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

    rerender_timestamp_ = metrics::CurTime();
    sprite_frame_change_timestamp_ = metrics::CurTime();
}

void RendererSystem::LaunchAnimationFrame(const ObjectAnimationData& animation_data, const Position& cur_pos,
                                          bool is_flipped) {

    typename SpriteSheet::StateFrame frame_pos =
        animation_data.sprite_sheet_->states_[animation_data.n_sprite_sheet_state_]
            .positions_[animation_data.cur_frame_];

    igraphicslib::Rect rect(frame_pos.x_, frame_pos.y_, frame_pos.w_, frame_pos.h_);
    animation_data.sprite_sheet_->sprite_.SetTexture(animation_data.sprite_sheet_->texture_);
    auto tmp = animation_data.sprite_sheet_->sprite_.Crop(rect);

    int x_coord = static_cast<int>(cur_pos.x_) - frame_pos.x_offset_;
    int y_coord =
        static_cast<int>(static_cast<int>(window_.GetSurfRect().h) - static_cast<int>(frame_pos.h_) * kSpriteScale) -
        (static_cast<int>(cur_pos.y_));

    if (is_flipped) {
        tmp.SetScale(-kSpriteScale, kSpriteScale);
        x_coord += static_cast<int>(frame_pos.w_) * static_cast<int>(kSpriteScale);
    } else {
        tmp.SetScale(kSpriteScale, kSpriteScale);
    }

    window_.DrawSprite({x_coord, y_coord}, tmp);
}

void RendererSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
    if (metrics::CheckDuration(rerender_timestamp_, metrics::kNSecForGapBetweenRenders)) {
        return;
    }

    bool is_frame_change = false;
    if (!metrics::CheckDuration(sprite_frame_change_timestamp_, metrics::kNSecForGapBetweenFrames)) {
        is_frame_change = true;
    }

    window_.Clear();

    for (auto target : inspected_entities_) {
        if (target.HasComponent<ObjectAnimationData>()) {
            auto cur_pos = *target.GetComponent<Position>();
            auto player_animation_data = target.GetComponent<ObjectAnimationData>().Get();
            auto frame_data = *target.GetComponent<RenderFrameData>();

            LaunchAnimationFrame(*player_animation_data, cur_pos, frame_data.is_flipped_);

            if (is_frame_change) {
                if (player_animation_data->UpdateFrame() && frame_data.idle_request_) {
                    player_animation_data->n_sprite_sheet_state_ = frame_data.n_new_state_;
                }
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
    if (inspected_entities_.find(event.entity_) == inspected_entities_.end()) {
        return;
    }

    // todo: implement something like playerStateDispatcherSystem
    if (event.cmd_ == PlayerCommand::TEXT_INSERT_REQUEST) {
        return;
    }

    // no actions should be if state states the same
    if (event.cmd_ == cur_player_state_) {
        return;
    }

    assert(event.entity_.HasComponent<RenderFrameData>());

    ecs::Entity entity = event.entity_;
    auto frame_data = entity.GetComponent<RenderFrameData>().Get();
    frame_data->idle_request_ = false;

    PlayerCommand event_cmd = event.cmd_;

    switch (event_cmd) {
        case PlayerCommand::IDLE: {
            cur_player_state_ = PlayerCommand::INVALID;
            frame_data->idle_request_ = true;

            break;
        }
        case PlayerCommand::WALK_LEFT: {
            frame_data->is_flipped_ = true;
            event_cmd = PlayerCommand::WALK_RIGHT;

            break;
        }
        case PlayerCommand::WALK_RIGHT: {
            frame_data->is_flipped_ = false;

            break;
        }
        case PlayerCommand::INVALID:
        case PlayerCommand::ATTACK_ONE:
        case PlayerCommand::ATTACK_TWO:
        default:
            cur_player_state_ = event_cmd;
    }

    for (auto target : inspected_entities_) {
        if (!target.HasComponent<PlayerTag>() || !target.HasComponent<ObjectAnimationData>()) {
            continue;
        }

        ObjectAnimationData* animation_storage = target.GetComponent<ObjectAnimationData>().Get();

        if (animation_storage->sprite_sheet_ == nullptr) {
            std::cout << "player doesn't have attached spritesheet, so new state won't appear on da screan\n";
            return;
        }

        std::string str_cmd = command_converter_[event_cmd];
        uint n_state_in_sprite_sheet = UINT32_MAX;

        size_t n_states_in_spritesheet = animation_storage->sprite_sheet_->states_.size();
        std::vector<typename SpriteSheet::StateData>* sprite_sheet_states = &animation_storage->sprite_sheet_->states_;

        for (uint n_state = 0; n_state < n_states_in_spritesheet; n_state++) {
            if (sprite_sheet_states->at(n_state).name_ == str_cmd) {
                n_state_in_sprite_sheet = n_state;
                break;
            }
        }

        if (n_state_in_sprite_sheet == UINT32_MAX) {
            std::cout << "state data not found on xml " << animation_storage->sprite_sheet_->img_path_ << ", exiting\n";
            assert(0);
        }

        if (frame_data->idle_request_) {
            frame_data->n_new_state_ = n_state_in_sprite_sheet;
        } else {
            animation_storage->n_sprite_sheet_state_ = n_state_in_sprite_sheet;
            animation_storage->cur_frame_ = 0;
        }
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
    // RenderFrameData data = {
    //     .cur_player_state_     = PLAYER_CMD::IDLE,
    //     .entity_        = event.entity_,
    //     .n_new_state_   = 0,
    //     .idle_request_  = true,
    //     .is_flipped_    = false
    // };

    RenderFrameData data = {.n_new_state_ = 0, .idle_request_ = true, .is_flipped_ = false};

    ecs::Entity entity = event.entity_;

    entity.Assign<RenderFrameData>(data);
    inspected_entities_.insert(event.entity_);
}

// void RendererSystem::Recieve(const LandingEvent& event) {
//     log(INFO, "{} landed\n", event.target_.GetId().GetIndex());
// }

// void RendererSystem::Recieve(const MovementStopEvent& event) {
//     inspected_entities_.erase(event.target_);

//     log(INFO, "{} stopped moving\n", event.target_.GetId().GetIndex());
// }
