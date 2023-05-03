#ifndef H_COLLISION_CMPNTS
#define H_COLLISION_CMPNTS

#include <cstdint>

struct Collidable {
    explicit Collidable(int64_t width = 0, int64_t height = 0) : width_(width), height_(height) {
    }

    int64_t width_;
    int64_t height_;
};

#endif
