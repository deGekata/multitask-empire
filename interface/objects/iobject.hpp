#ifndef MULTITASK_EMPIRE_INTERFACE_OBJECTS_IOBJECT_HPP
#define MULTITASK_EMPIRE_INTERFACE_OBJECTS_IOBJECT_HPP

// Multitask-Empire
#include <events/player_events.hpp>

namespace interface::objects {

class IObject {
public:
	virtual ~IObject() = default;

	// Mouse Events
	virtual bool OnMouseMove  (igraphicslib::MouseMoveEventData  mouse_data) = 0;
	virtual bool OnWheelScroll(igraphicslib::MouseWheelEventData wheel_data) = 0;

	virtual bool OnButtonPress  (igraphicslib::MouseButtonEventData button_data) = 0;
	virtual bool OnButtonRelease(igraphicslib::MouseButtonEventData button_data) = 0;

	// Keyboard Events
	virtual bool OnKeyPress  (igraphicslib::KeyEventData key_data) = 0;
	virtual bool OnKeyRelease(igraphicslib::KeyEventData key_data) = 0;

	// Time Event
	virtual bool OnTimerTick(utility::Time time_data) = 0;
};

}  // namespace objects

#endif  // MULTITASK_EMPIRE_INTERFACE_OBJECTS_IOBJECT_HPP
