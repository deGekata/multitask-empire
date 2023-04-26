#ifndef H_MOVEMENT_CMPNTS
#define H_MOVEMENT_CMPNTS

#include <cstdint>

struct Position {
    int64_t x_;
    int64_t y_;
};

struct Velocity {
    int64_t vx_;
    int64_t vy_;
};

struct Acceleration {
    int64_t ax_;
    int64_t ay_;
};

#endif
