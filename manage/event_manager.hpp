#ifndef MULTITASK_EMPIRE_MANAGE_EVENT_MANAGER_HPP
#define MULTITASK_EMPIRE_MANAGE_EVENT_MANAGER_HPP

// Multitask-Empire
#include <objects/object.hpp>

// C standart library
#include <cstdint>

// C++ standart library
#include <algorithm>
#include <utility>

// Containers
#include <map>

namespace manage {

class EventManager final : public ecs::Receiver<EventManager> {
	using Priority = uint64_t;

public:
	EventManager();
	explicit EventManager(EventManager* previous);

	~EventManager() final;

	static EventManager* Current();

	// O(log N)
	void Subscribe(objects::IObject* object, Priority priority);
	EventManager& operator+=(std::pair<Priority, objects::IObject*> subscriber);

	// O(N)
	void Unsubscribe(objects::IObject* object);
	EventManager& operator-=(objects::IObject* object);

	// One-shot
	EventManager* Release();

	// Receiver

	// Keyboard events
	void Receive(const KeyPressedEvent& event);
	void Receive(const KeyReleasedEvent& event);

	// Mosue events
	void Receive(const MouseMovedEvent& event);
	void Receive(const MouseButtonPressedEvent& event);
	void Receive(const MouseButtonReleasedEvent& event);
	void Receive(const MouseWheelScrolledEvent& event);

	// Timer event
	void Receive(const TimerTickedEvent& event);

private:
	EventManager* previous_;
	static EventManager* current;

	std::multimap<Priority, objects::IObject*> subscribers_;
};

}  // namespace managers

#endif  // MULTITASK_EMPIRE_MANAGE_EVENT_MANAGER_HPP
