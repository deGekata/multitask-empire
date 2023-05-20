#include <interface/manage/children_manager.hpp>

#include <interface/manage/base_manager.hpp>

namespace interface::manage {

ChildrenManager::ChildrenManager()
	: manager_impl_(new BaseManager()) {
}

ChildrenManager::~ChildrenManager() {
	delete manager_impl_;
}

void ChildrenManager::Subscribe(objects::IObject* object, Priority priority) {
	manager_impl_->Subscribe(object, priority);
}

void ChildrenManager::Unsubscribe(objects::IObject* object) {
	manager_impl_->Unsubscribe(object);
}

void ChildrenManager::ProcessKeyPress(igraphicslib::KeyEventData key) {
	manager_impl_->ProcessKeyPress(key);
}

void ChildrenManager::ProcessKeyRelease(igraphicslib::KeyEventData key) {
	manager_impl_->ProcessKeyRelease(key);
}

void ChildrenManager::ProcessMouseMovement(igraphicslib::MouseMoveEventData move) {
	manager_impl_->ProcessMouseMovement(move);
}

void ChildrenManager::ProcessButtonPress(igraphicslib::MouseButtonEventData button) {
	manager_impl_->ProcessButtonPress(button);
}

void ChildrenManager::ProcessButtonRelease(igraphicslib::MouseButtonEventData button) {
	manager_impl_->ProcessButtonRelease(button);
}

void ChildrenManager::ProcessWheelScroll(igraphicslib::MouseWheelEventData wheel) {
	manager_impl_->ProcessWheelScroll(wheel);
}

void ChildrenManager::ProcessTimerTick(utility::Time time) {
	manager_impl_->ProcessTimerTick(time);
}

}  // namespace managers
