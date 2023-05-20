#ifndef H_BATTLE_CMPNTS
#define H_BATTLE_CMPNTS

#include <functional>
#include <list>

#include <ecs/config.hpp>
#include <ecs/entity.hpp>

struct PassiveCurseInfo {
    ecs::TimeDelta time_left_;
    std::function<void(ecs::Entity)> remove_action_;
};

struct PassiveCursesStorage {
    std::list<PassiveCurseInfo> storage_;
};

struct ActiveCurseInfo {
    ecs::TimeDelta time_left_;
    std::function<void(ecs::Entity)> periodic_action_;
};

struct ActiveCursesStorage {
    std::list<ActiveCurseInfo> storage_;
};

struct BlockedTag {};

struct BlockReserve {
    double durability_;
};

struct DamagerTag {
    ecs::Entity creator_;
};

struct Health {
    double health_;
};

struct Special {
    double experience_;
};

struct SpecialAbility {
    enum class Type {
        Fireball,
        Slime,
        Potion
    };

    Type type_;
};

struct AttackSpeed {
    double speed_;
};

struct AttackDistance {
    double distance_;
};

struct AttackPower {
    double power_;
};

#endif
