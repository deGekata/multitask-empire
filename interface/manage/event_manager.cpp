#include <interface/manage/event_manager.hpp>

#include <interface/manage/base_manager.hpp>

namespace interface::manage {

EventManager* EventManager::current = nullptr;

EventManager::EventManager()
	: previous_(EventManager::Current())
	, manager_impl_(new BaseManager()) {
	current = this;
}

EventManager::~EventManager() {
	delete manager_impl_;
	current = Release();
}

EventManager* EventManager::Current() {
	return current;
}

void EventManager::Subscribe(objects::IObject* object, Priority priority) {
	manager_impl_->Subscribe(object, priority);
}

void EventManager::Unsubscribe(objects::IObject* object) {
	manager_impl_->Unsubscribe(object);
}

EventManager* EventManager::Release() {
	return std::exchange(previous_, nullptr);
}

void EventManager::ProcessKeyPress(igraphicslib::KeyEventData key) {
	if (current != this) {
		return;
	}

	manager_impl_->ProcessKeyPress(key);
}

void EventManager::ProcessKeyRelease(igraphicslib::KeyEventData key) {
	if (current != this) {
		return;
	}

	manager_impl_->ProcessKeyRelease(key);
}

void EventManager::ProcessMouseMovement(igraphicslib::MouseMoveEventData move) {
	if (current != this) {
		return;
	}

	manager_impl_->ProcessMouseMovement(move);
}

void EventManager::ProcessButtonPress(igraphicslib::MouseButtonEventData button) {
	if (current != this) {
		return;
	}

	manager_impl_->ProcessButtonPress(button);
}

void EventManager::ProcessButtonRelease(igraphicslib::MouseButtonEventData button) {
	if (current != this) {
		return;
	}

	manager_impl_->ProcessButtonRelease(button);
}

void EventManager::ProcessWheelScroll(igraphicslib::MouseWheelEventData wheel) {
	if (current != this) {
		return;
	}

	manager_impl_->ProcessWheelScroll(wheel);
}

void EventManager::ProcessTimerTick(utility::Time time) {
	manager_impl_->ProcessTimerTick(time);
}

}  // namespace managers
