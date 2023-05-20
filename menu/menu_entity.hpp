/**
 * @defgroup   MENU_ENTITY menu entity
 *
 * @brief      This file describes main menu entity.
 *
 * @author     Stanislav
 * @date       2023
 */

#ifndef MULTITASK_EMPIRE_MENU_ENTITY_HPP
#define MULTITASK_EMPIRE_MENU_ENTITY_HPP

// This project
#include <ecs/quick.hpp>

// Indicates that the given entity is menu
struct MenuTag {};

struct MenuData {
	bool is_visible{false};
};

#endif  // MULTITASK_EMPIRE_MENU_ENTITY_HPP
