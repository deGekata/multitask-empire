#ifndef H_BATTLE_CMPNTS
#define H_BATTLE_CMPNTS

#include <ecs/config.hpp>
#include <ecs/entity.hpp>

struct DamagerTag {
    ecs::Entity creator_;
};

struct Health {
    int32_t health_;
};

struct AttackSpeed {
    int32_t speed_;
};

struct AttackDistance {
    int32_t distance_;
};

struct AttackPower {
    int32_t power_;
};

#endif
