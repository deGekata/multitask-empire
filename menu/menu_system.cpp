// This project
#include <menu/menu_system.hpp>
#include <menu/menu_entity.hpp>

namespace menu {

void MenuSystem::Configure(ecs::EntityManager& entity_manager, ecs::EventManager& event_manager) {
	ecs::Entity menu = entity_manager.Create();
	menu.Assign<MenuTag>();
	menu.Assign<MenuData>();

	event_manager.Subscribe<KeyReleasedEvent>(*this);
	event_manager.Subscribe<KeyPressedEvent> (*this);
}

void MenuSystem::Update(ecs::EntityManager& entity_manager, ecs::EventManager& event_manager, ecs::TimeDelta dt) {
}

}  // namespace menu
