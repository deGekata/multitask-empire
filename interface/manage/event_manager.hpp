#ifndef MULTITASK_EMPIRE_INTERFACE_MANAGE_EVENT_MANAGER_HPP
#define MULTITASK_EMPIRE_INTERFACE_MANAGE_EVENT_MANAGER_HPP

// Multitask-Empire
#include <interface/objects/iobject.hpp>

namespace interface::manage {

class BaseManager;

class EventManager {
	using Priority = int64_t;

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
	EventManager* previous_;
	static EventManager* current;

	BaseManager* manager_impl_;
};

}  // namespace managers

#endif  // MULTITASK_EMPIRE_INTERFACE_MANAGE_EVENT_MANAGER_HPP
