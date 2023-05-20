#ifndef MULTITASK_EMPIRE_INTERFACE_MANAGE_CHILDREN_MANAGER_HPP
#define MULTITASK_EMPIRE_INTERFACE_MANAGE_CHILDREN_MANAGER_HPP

// Multitask-Empire
#include <interface/objects/iobject.hpp>

namespace interface::manage {

class BaseManager;

class ChildrenManager {
	using Priority = int64_t;

public:
	ChildrenManager();
	~ChildrenManager();

	// O(log N)
	void Subscribe(objects::IObject* object, Priority priority = 0);

	// O(N)
	void Unsubscribe(objects::IObject* object);

	// Keyboard events
	void ProcessKeyPress  (igraphicslib::KeyEventData key);
	void ProcessKeyRelease(igraphicslib::KeyEventData key);

	// Mosue events
	void ProcessMouseMovement(igraphicslib::MouseMoveEventData move);
	void ProcessWheelScroll(igraphicslib::MouseWheelEventData wheel);

	void ProcessButtonPress  (igraphicslib::MouseButtonEventData button);
	void ProcessButtonRelease(igraphicslib::MouseButtonEventData button);

	// Timer event
	void ProcessTimerTick(utility::Time time);

private:
	BaseManager* manager_impl_;
};

}  // namespace interface::manage

#endif  // MULTITASK_EMPIRE_INTERFACE_MANAGE_CHILDREN_MANAGER_HPP
