#include <physics/limiter.hpp>

#include <components/movement_components.hpp>
#include <components/player_components.hpp>

static constexpr double kXMin = 0.0;
static constexpr double kXMax = 1800.0;

void LimiterSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {

}

void LimiterSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
    entities.Each<PBattleAbleAttributes, Position, Velocity>([](ecs::Entity, PBattleAbleAttributes&,  Position& position, Velocity& velocity) {
        position.x_ = std::max(kXMin, position.x_);
        position.x_ = std::min(kXMax, position.x_); 
    });
}
