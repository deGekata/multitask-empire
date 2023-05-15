#ifndef H_BACKGROUND_SYSTEM
#define H_BACKGROUND_SYSTEM

#include <iostream>
#include <string>

#include <ecs/system.hpp>

class BackgroundSystem : public ecs::System<BackgroundSystem>{
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
};

#endif
