#ifndef H_GRAPHIC_CMPNTS
#define H_GRAPHIC_CMPNTS

#include <string>
#include <vector>

struct SpriteSheet {

    struct StateFrame {
        int x_, y_, h_, w_;
    };

    struct StateData {
        std::string             name_;
        std::vector<StateFrame> positions_;
    };

    // std::string            name_;
    std::string            img_path_;
    std::vector<StateData> states_;
};

struct SpriteSheetStorageTag {};

#endif // H_GRAPHIC_CMPNTS
