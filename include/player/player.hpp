#ifndef H_PLAYER
#define H_PLAYER

#include <iostream>
#include <string>

#include <ecs/system.hpp>

class PlayerSystem : public ecs::System<PlayerSystem>{
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
};

#endif
