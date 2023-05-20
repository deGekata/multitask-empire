#ifndef MULTITASK_EMPIRE_INTERFACE_MANAGE_BASE_MANAGER_HPP
#define MULTITASK_EMPIRE_INTERFACE_MANAGE_BASE_MANAGER_HPP

// Multitask-Empire
#include <interface/objects/iobject.hpp>

// C standart library
#include <cstdint>

// C++ standart library
#include <algorithm>
#include <utility>

// Containers
#include <map>

namespace interface::manage {

using Priority = int64_t;

class BaseManager {
public:
	BaseManager() = default;

	// O(log N)
	void Subscribe(objects::IObject* object, Priority priority = 0);

	// O(N)
	void Unsubscribe(objects::IObject* object);

	// Keyboard events
	bool ProcessKeyPress  (igraphicslib::KeyEventData key);
	bool ProcessKeyRelease(igraphicslib::KeyEventData key);

	// Mosue events
	bool ProcessMouseMovement(igraphicslib::MouseMoveEventData move);
	bool ProcessWheelScroll(igraphicslib::MouseWheelEventData wheel);

	bool ProcessButtonPress  (igraphicslib::MouseButtonEventData button);
	bool ProcessButtonRelease(igraphicslib::MouseButtonEventData button);

	// Timer event
	bool ProcessTimerTick(utility::Time time);

protected:
	std::multimap<Priority, objects::IObject*> subscribers_;
};

}  // namespace interface::manage

#endif  // MULTITASK_EMPIRE_INTERFACE_MANAGE_BASE_MANAGER_HPP
