/**
 * @defgroup   MENU menu
 *
 * @brief      This file describes main menu system.
 *
 * @author     Stanislav
 * @date       2023
 */

#ifndef MULTITASK_EMPIRE_MENU_SYSTEM_HPP
#define MULTITASK_EMPIRE_MENU_SYSTEM_HPP

// This project
#include <ecs/quick.hpp>
#include <player/input.hpp>

namespace menu {

class MenuSystem : public ecs::System<MenuSystem>, public ecs::Receiver<MenuSystem> {
public:
	MenuSystem() = default;

	// System
    void Configure(ecs::EntityManager& entity_manager, ecs::EventManager& event_manager) final;
    void Update(ecs::EntityManager& entity_manager, ecs::EventManager& event_manager, ecs::TimeDelta dt) final;

    // Receiver
    void Receive(const KeyReleasedEvent& event);
    void Receive(const KeyPressedEvent& event);

private:
	bool is_shown_;
};

}  // namespace menu

#endif  // MULTITASK_EMPIRE_MENU_SYSTEM_HPP
