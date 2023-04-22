#ifndef H_GRAVITATION
#define H_GRAVITATION

#include <ecs/system.hpp>

constexpr int64_t kAttraction = 20;

class GravitationSystem : public ecs::System<GravitationSystem> {
public:
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
};

#endif
