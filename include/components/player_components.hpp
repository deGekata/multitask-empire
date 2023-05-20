#ifndef H_PLAYER_CMPNTS
#define H_PLAYER_CMPNTS

#include <graphics/event.hpp>

struct PlayerTag {};

enum class ActionCommandType : int {
    Invalid = -1,
    RunRight,
    RunLeft,
    Jump,
    Attack,
    Block,
    Special,
    Idle,
    StopRunningRight,
    StopRunningLeft
};

enum class PlayerCommandType : int {
    Invalid = -1,
    Action,
    TextInsertRequest
};

struct PlayerCommand {
    PlayerCommandType   type_;
};

struct ActionCommand {
    ActionCommandType   type_;
};

struct BattleAbleAttributesStorageTag {};

struct BattleAbleAttributes {

    std::string name_;
    std::string sprite_sheet_path_;
    std::string xml_path_;
    
    uint        hp_lvl_;
    double      jmp_ratio_;
    double      speed_ratio_;

    struct basicFrames {
        std::string run_;
        std::string idle_;
        std::string death_;
        std::string jump_;
    } basic_frames_;

    std::vector<std::string> replics_;

    struct attackData {
        std::string xml_name_;
        std::string name_;
        igraphicslib::KeyboardKey binded_key_;
        uint damage_value_;
        // effect?
    };

    std::vector<attackData> attacks_;

    std::string config_path_;
};

struct PBattleAbleAttributes {
    BattleAbleAttributes* attr_;
};

#endif
