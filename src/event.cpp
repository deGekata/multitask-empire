#include "event.hpp"
#include "utility/non_copiable.hpp"

namespace ecs {

EventBase::FamilyType EventBase::family_counter_ = 0;

EventBase::EventBase() {
}

EventBase::~EventBase() {
}

ReceiverBase::ReceiverBase()
	: connections_() {
}

ReceiverBase::~ReceiverBase() {
	for (auto connection : connections_) {
	    auto &ptr = connection.second.first;
	    if (!ptr.expired()) {
	        ptr.lock()->Disconnect(connection.second.second);
	    }
	}
}

size_t ReceiverBase::SignalsCount() const {
	std::size_t size = 0;
	for (auto connection : connections_) {
	    if (!connection.second.first.expired()) {
	        size++;
	    }
	}
	return size;
}

EventManager::EventManager()
	: utility::NonCopiable() {
}

EventManager::~EventManager() {
}

template <typename EventType, typename RecieverType>
void EventManager::Subscribe(RecieverType& reciever) {
	static_assert(Receivable<RecieverType, EventType>, "Irreceivable type.");
}

template <typename EventType, typename RecieverType>
void EventManager::Unsubscribe(RecieverType& reciever) {
}

template <typename EventType>
void EventManager::Emit(const EventType& event) {
}

template <typename EventType>
void EventManager::Emit(std::unique_ptr<EventType> event) {
}

template <typename EventType, typename... ArgTypes>
void EventManager::Emit(ArgTypes&&... args) {
}

size_t EventManager::RecieversCount() const {
}

} // namespace ecs
