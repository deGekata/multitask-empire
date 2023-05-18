#ifndef H_PLAYER_CMPNTS
#define H_PLAYER_CMPNTS

struct PlayerTag {};

enum class PlayerCommand : int {
    INVALID = -1,
    IDLE,
    WALK_LEFT,
    WALK_RIGHT,
    ATTACK_ONE,
    ATTACK_TWO,
    DEATH,
    JUMP,
    SPECIAL,
    BLOCK,
    TEXT_INSERT_REQUEST
};

struct LastPlayerCommand {
    PlayerCommand cmd;
};

enum PLAYER_STATE { DEFAULT, UPPER_ATTACK, LOWER_ATTACK, JUMP, DOWN, WALK };

struct PlayerState {
    PLAYER_STATE cur_state_;
};

#endif
