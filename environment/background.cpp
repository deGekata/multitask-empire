#include <environment/background.hpp>
#include <components/environment_components.hpp>

void BackgroundSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    ecs::Entity background = entities.Create();
    background.Assign<Background>();

    //! remove
    //???
    // events.Emit<SkinChangeRequest>("./assets/sprites/orc_berserk.png", background);
    // events.Emit<PlayerCommandEvent>(PlayerCommand::IDLE, player);
}

void BackgroundSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}
