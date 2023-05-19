#ifndef H_BATTLE_CMPNTS
#define H_BATTLE_CMPNTS

#include <ecs/config.hpp>
#include <ecs/entity.hpp>

struct BlockedTag {
    ecs::TimeDelta remain_time_;
};

struct DamagerTag {
    ecs::Entity creator_;
};

struct Health {
    double health_;
};

struct SpecialAbility {
    enum class Type {
        Fireball,
        Slime,
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
