#ifndef H_COLLISION_CMPNTS
#define H_COLLISION_CMPNTS

#include <cstdint>

struct CollidableTag {};

struct HitBox {
    explicit HitBox(double width = 0.0, double height = 0.0) : width_(width), height_(height) {
    }

    double width_;
    double height_;
};

#endif
