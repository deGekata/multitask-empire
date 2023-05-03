#ifndef H_GRAPHIC_CMPNTS
#define H_GRAPHIC_CMPNTS

#include <string>
#include <vector>

#include <graphics/aSprite.hpp>
#include <graphics/aTexture.hpp>

struct SpriteSheet {

    struct StateFrame {
        uint x_, y_, h_, w_;
        int  x_offset_, y_offset_;
        uint frame_w_, frame_h_;
    };

    struct StateData {
        std::string             name_;
        std::vector<StateFrame> positions_;
    };

    // std::string            name_;
    igraphicslib::Texture  texture_;
    std::string            img_path_;
    igraphicslib::Sprite   sprite_;
    std::vector<StateData> states_;
};

struct SpriteSheetStorageTag {};

struct ObjectAnimationData {
    SpriteSheet* sprite_sheet_;
    uint         n_sprite_sheet_state_;
    uint         cur_frame_;
};

struct RenderFrameData {
    uint        n_new_state_;
    bool        idle_request_;
    bool        is_flipped_;
};

#endif // H_GRAPHIC_CMPNTS
