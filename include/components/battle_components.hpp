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
    int32_t health_;
};

struct SpecialAbility {
    enum class Type {
        Fireball,
        Slime,
    };

    Type type_;
};

struct AttackSpeed {
    AttackSpeed() : speed_multiplier(1.0) {
    }

    double speed_multiplier;
};

struct AttackDistance {
    AttackDistance() : distance_multiplier(1.0) {
    }

    double distance_multiplier;
};

struct AttackPower {
    explicit AttackPower(double multiplier = 1.0) : power_multiplier(multiplier) {
    }

    double power_multiplier;
};

#endif
