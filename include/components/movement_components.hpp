#ifndef H_MOVEMENT_CMPNTS
#define H_MOVEMENT_CMPNTS

#include <cstdint>

struct Position {
    double x_;
    double y_;
};

struct Velocity {
    double vx_;
    double vy_;
};

struct Acceleration {
    double ax_;
    double ay_;
};

struct Rotation {
    bool is_flipped_;
};

#endif
