#include <manage/event_manager.hpp>

namespace manage {

EventManager::EventManager()
	: previous_(nullptr) {
}

EventManager::EventManager(EventManager* previous)
	: previous_(previous) {
}

void EventManager::Subscribe(objects::IObject* object, Priority priority) {
	subscribers_.emplace(priority, object);
}

EventManager& EventManager::operator+=(std::pair<Priority, objects::IObject*> subscriber) {	
	Subscribe(subscriber.second, subscriber.first);
	return *this;
}

void EventManager::Unsubscribe(objects::IObject* object) {
	std::erase_if(subscribers_, [&](const std::pair<Priority, objects::IObject*>& subscriber) {
		return subscriber.second == object;
	});
}

EventManager& EventManager::operator-=(objects::IObject* object) {
	Unsubscribe(object);
	return *this;
}

EventManager* EventManager::Release() {
	return std::exchange(previous_, nullptr);
}

void EventManager::Receive(const KeyPressedEvent& event) {
	for (auto& subscriber : subscribers_) {
		subscriber.second->OnKeyPress(event.data_);
	}
}

void EventManager::Receive(const KeyReleasedEvent& event) {
	for (auto& subscriber : subscribers_) {
		subscriber.second->OnKeyRelease(event.data_);
	}
}

void EventManager::Receive(const MouseMovedEvent& event) {
	for (auto& subscriber : subscribers_) {
		subscriber.second->OnMouseMove(event.mouse_data);
	}
}

void EventManager::Receive(const MouseButtonPressedEvent& event) {
	for (auto& subscriber : subscribers_) {
		subscriber.second->OnButtonPress(event.button_data);
	}
}

void EventManager::Receive(const MouseButtonReleasedEvent& event) {
	for (auto& subscriber : subscribers_) {
		subscriber.second->OnButtonRelease(event.button_data);
	}
}

void EventManager::Receive(const MouseWheelScrolledEvent& event) {
	for (auto& subscriber : subscribers_) {
		subscriber.second->OnWheelScroll(event.wheel_data);
	}
}

void EventManager::Receive(const TimerTickedEvent& event) {
	for (auto& subscriber : subscribers_) {
		subscriber.second->OnTimerTick(event.time_data);
	}
}

}  // namespace managers
