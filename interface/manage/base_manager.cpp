#include <interface/manage/base_manager.hpp>

namespace interface::manage {

void BaseManager::Subscribe(objects::IObject* object, Priority priority) {
	subscribers_.emplace(priority, object);
}

void BaseManager::Unsubscribe(objects::IObject* object) {
	std::erase_if(subscribers_, [&](const std::pair<Priority, objects::IObject*>& subscriber) {
		return subscriber.second == object;
	});
}

bool BaseManager::ProcessKeyPress(igraphicslib::KeyEventData key) {
	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnKeyPress(key)) {
			return true;
		}
	}

	return false;
}

bool BaseManager::ProcessKeyRelease(igraphicslib::KeyEventData key) {
	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnKeyRelease(key)) {
			return true;
		}
	}

	return false;
}

bool BaseManager::ProcessMouseMovement(igraphicslib::MouseMoveEventData move) {
	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnMouseMove(move)) {
			return true;
		}
	}

	return false;
}

bool BaseManager::ProcessButtonPress(igraphicslib::MouseButtonEventData button) {
	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnButtonPress(button)) {
			return true;
		}
	}

	return false;
}

bool BaseManager::ProcessButtonRelease(igraphicslib::MouseButtonEventData button) {
	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnButtonRelease(button)) {
			return true;
		}
	}

	return false;
}

bool BaseManager::ProcessWheelScroll(igraphicslib::MouseWheelEventData wheel) {
	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnWheelScroll(wheel)) {
			return true;
		}
	}

	return false;
}

bool BaseManager::ProcessTimerTick(utility::Time time) {
	for (auto& subscriber : subscribers_) {
		subscriber.second->OnTimerTick(time);
	}

	return true;
}

}  // namespace interface::manage
