#ifndef H_GRAPHIC_CMPNTS
#define H_GRAPHIC_CMPNTS

#include <string>
#include <vector>

#include <graphics/aSprite.hpp>
#include <graphics/aTexture.hpp>

struct SpriteSheet {
    struct StateFrame {
        uint x_, y_, h_, w_;
        int x_offset_, y_offset_;
        uint frame_w_, frame_h_;
    };

    struct StateData {
        std::string name_;
        std::vector<StateFrame> positions_;
    };

    // std::string            name_;
    igraphicslib::Texture texture_;
    std::string img_path_;
    igraphicslib::Sprite sprite_;
    std::vector<StateData> states_;
};

struct SpriteSheetStorageTag {};
#include <map>

struct ObjectSpriteSheet {
    SpriteSheet*        sprite_sheet_;
    std::map<int, uint> id_to_n_state_mapping_;
};

struct ObjectAnimationData {
    // todo: rename
    ObjectSpriteSheet sprite_sheet_;
    
    uint n_sprite_sheet_state_;
    uint cur_frame_;

    // @brief Updates frame and returns true if animation is ended
    bool UpdateFrame() {
        assert(sprite_sheet_.sprite_sheet_);

        cur_frame_++;
        if (cur_frame_ == sprite_sheet_.sprite_sheet_->states_[n_sprite_sheet_state_].positions_.size()) {
            cur_frame_ = 0;

            return true;
        }
        return false;
    }
};

struct RenderFrameData {
    uint n_new_state_;
    bool idle_request_;
};

#endif  // H_GRAPHIC_CMPNTS
