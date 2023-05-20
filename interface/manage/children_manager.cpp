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

bool ChildrenManager::ProcessKeyPress(igraphicslib::KeyEventData key) {
	return manager_impl_->ProcessKeyPress(key);
}

bool ChildrenManager::ProcessKeyRelease(igraphicslib::KeyEventData key) {
	return manager_impl_->ProcessKeyRelease(key);
}

bool ChildrenManager::ProcessMouseMovement(igraphicslib::MouseMoveEventData move) {
	return manager_impl_->ProcessMouseMovement(move);
}

bool ChildrenManager::ProcessButtonPress(igraphicslib::MouseButtonEventData button) {
	return manager_impl_->ProcessButtonPress(button);
}

bool ChildrenManager::ProcessButtonRelease(igraphicslib::MouseButtonEventData button) {
	return manager_impl_->ProcessButtonRelease(button);
}

bool ChildrenManager::ProcessWheelScroll(igraphicslib::MouseWheelEventData wheel) {
	return manager_impl_->ProcessWheelScroll(wheel);
}

bool ChildrenManager::ProcessTimerTick(utility::Time time) {
	return manager_impl_->ProcessTimerTick(time);
}

}  // namespace managers
