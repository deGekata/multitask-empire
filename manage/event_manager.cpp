#include <manage/event_manager.hpp>

namespace manage {

EventManager* EventManager::current = nullptr;

EventManager::EventManager()
	: previous_(nullptr) {
	current = this;
}

EventManager::EventManager(EventManager* previous)
	: previous_(previous) {
	current = this;
}

EventManager::~EventManager() {
	current = Release();
}

EventManager* EventManager::Current() {
	return current;
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
	if (current != this) {
		return;
	}

	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnKeyPress(event.data_)) {	
			return;
		}
	}
}

void EventManager::Receive(const KeyReleasedEvent& event) {
	if (current != this) {
		return;
	}

	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnKeyRelease(event.data_)) {
			return;
		}
	}
}

void EventManager::Receive(const MouseMovedEvent& event) {
	if (current != this) {
		return;
	}

	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnMouseMove(event.mouse_data)) {
			return;
		}
	}
}

void EventManager::Receive(const MouseButtonPressedEvent& event) {
	if (current != this) {
		return;
	}

	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnButtonPress(event.button_data)) {
			return;
		}
	}
}

void EventManager::Receive(const MouseButtonReleasedEvent& event) {
	if (current != this) {
		return;
	}

	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnButtonRelease(event.button_data)) {
			return;
		}
	}
}

void EventManager::Receive(const MouseWheelScrolledEvent& event) {
	if (current != this) {
		return;
	}

	for (auto& subscriber : subscribers_) {
		if (bool stop = subscriber.second->OnWheelScroll(event.wheel_data)) {
			return;
		}
	}
}

void EventManager::Receive(const TimerTickedEvent& event) {
	if (current != this) {
		return;
	}

	for (auto& subscriber : subscribers_) {
		subscriber.second->OnTimerTick(event.time_data);
	}
}

}  // namespace managers
