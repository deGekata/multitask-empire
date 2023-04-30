#ifndef H_PLAYER_CMPNTS
#define H_PLAYER_CMPNTS

struct PlayerTag {};

enum PLAYER_STATE{
    DEFAULT,
    UPPER_ATTACK,
    LOWER_ATTACK,
    JUMP,
    DOWN
};

struct PlayerState {
    PLAYER_STATE cur_state_;
};

#endif
