#ifndef H_GRAPHIC_CMPNTS
#define H_GRAPHIC_CMPNTS

#include <string>
#include <vector>

#include <graphics/aSprite.hpp>

struct SpriteSheet {

    struct StateFrame {
        uint x_, y_, h_, w_;
    };

    struct StateData {
        std::string             name_;
        std::vector<StateFrame> positions_;
    };

    // std::string            name_;
    std::string            img_path_;
    igraphicslib::Sprite   sprite_;
    std::vector<StateData> states_;
};

struct SpriteSheetStorageTag {};

struct ObjectAnimationData {
    SpriteSheet* sprite_sheet_;
    int          n_sprite_sheet_state_;
    int          cur_frame_;
};

#endif // H_GRAPHIC_CMPNTS
