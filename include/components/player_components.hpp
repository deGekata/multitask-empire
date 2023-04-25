#ifndef H_PLAYER_CMPNTS
#define H_PLAYER_CMPNTS

#include <string>

struct PlayerTag {};

struct PendingPlayerCommand {
    std::string command_;
};

enum class PlayerActions {
    Left,
    Right,
    Jump,
};


#endif
