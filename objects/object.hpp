#ifndef MULTITASK_EMPIRE_OBJECTS_OBJECT_HPP
#define MULTITASK_EMPIRE_OBJECTS_OBJECT_HPP

// Multitask-Empire
#include <events/player_events.hpp>

namespace objects {

class IObject {
public:
	virtual ~IObject() = 0;

	// Mouse Events
	virtual bool OnMouseMove(igraphicslib::MouseMoveEventData mouse_data) = 0;
	virtual bool OnWheelScroll(igraphicslib::MouseWheelEventData wheel_data) = 0;

	virtual bool OnButtonPress  (igraphicslib::MouseButtonEventData button_data) = 0;
	virtual bool OnButtonRelease(igraphicslib::MouseButtonEventData button_data) = 0;

	// Keyboard Events
	virtual bool OnKeyPress  (igraphicslib::KeyEventData key_data) = 0;
	virtual bool OnKeyRelease(igraphicslib::KeyEventData key_data) = 0;

	// Time Event
	virtual bool OnTimerTick(igraphicslib::Event::Timepoint time_data) = 0;
};

class IWidget : public IObject {
public:
	~IWidget() override = 0;

	// Every widget knows how to draw itself
	virtual void Draw() = 0;

	virtual void Show() = 0;
	virtual void Hide() = 0;
};

}  // namespace objects

#endif  // MULTITASK_EMPIRE_OBJECTS_OBJECT_HPP
