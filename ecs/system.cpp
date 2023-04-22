#include <ecs/system.hpp>

namespace ecs {

BaseSystem::Family BaseSystem::family_counter_ = 0;

BaseSystem::~BaseSystem() {
}

void BaseSystem::Configure(EntityManager&, EventManager&) {
}

SystemManager::SystemManager(EntityManager& entity_manager, EventManager& event_manager)
    : entity_manager_(entity_manager), event_manager_(event_manager) {
}

SystemManager::~SystemManager() {
    for (auto& it : systems_) {
        delete it.second;
    }
}

void SystemManager::UpdateAll(TimeDelta dt) {
    assert(is_initialized_ && "SystemManager::configure() not called");

    for (auto& pair : systems_) {
        pair.second->Update(entity_manager_, event_manager_, dt);
    }
}

void SystemManager::Configure() {
    for (auto& pair : systems_) {
        pair.second->Configure(entity_manager_, event_manager_);
    }

    is_initialized_ = true;
}

};  // namespace ecs
