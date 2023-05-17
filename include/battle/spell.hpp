#ifndef H_SPELL
#define H_SPELL

#include <ecs/quick.hpp>

class SpellSystem : public ecs::System<SpellSystem>, public ecs::Reciever<SpellSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive();
};

#endif
