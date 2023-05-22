#ifndef H_ATTACH
#define H_ATTACH

#include <ecs/quick.hpp>

class AttachSystem : public ecs::System<AttachSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
};

#endif
