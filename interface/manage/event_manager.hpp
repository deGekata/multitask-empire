#ifndef MULTITASK_EMPIRE_INTERFACE_MANAGE_EVENT_MANAGER_HPP
#define MULTITASK_EMPIRE_INTERFACE_MANAGE_EVENT_MANAGER_HPP

// Multitask-Empire
#include <interface/objects/iobject.hpp>

namespace interface::manage {

class BaseManager;
using Priority = int64_t;

class EventManager {
public:
	EventManager();
	
	~EventManager();

	static EventManager* Current();

	// O(log N)
	void Subscribe(objects::IObject* object, Priority priority = 0);

	// O(N)
	void Unsubscribe(objects::IObject* object);

	// One-shot
	EventManager* Release();

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

private:
	EventManager* previous_;
	static EventManager* current;

	BaseManager* manager_impl_;
};

}  // namespace managers

#endif  // MULTITASK_EMPIRE_INTERFACE_MANAGE_EVENT_MANAGER_HPP
