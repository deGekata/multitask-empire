#ifndef H_PLAYER
#define H_PLAYER

#include <iostream>
#include <string>

#include <ecs/system.hpp>

class PlayerSystem : public ecs::System<PlayerSystem> {
public:
    void Configure(ecs::EntityManager& entities) override;
    void Update(ecs::EntityManager& entities, ecs::TimeDelta dt) override;
};

#endif
