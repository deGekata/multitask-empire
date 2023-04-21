#include "event.hpp"

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
	for (const auto& connection : connections_) {
	    std::weak_ptr<EventSignal> pointer = connection.second.first;
	    signal::CallBackId id = connection.second.second;
	    if (!pointer.expired()) {
	        pointer.lock()->Disconnect(id);
	    }
	}
}

size_t ReceiverBase::SignalsCount() const {
	size_t count = 0;
	for (const auto& connection : connections_) {
	    std::weak_ptr<EventSignal> pointer = connection.second.first;
	    if (!pointer.expired()) {
	        count++;
	    }
	}
	return count;
}

auto ReceiverBase::ConnectionsList() {
	return connections_;
}

EventManager::EventManager()
	: utility::NonCopiable() {
}

EventManager::~EventManager() {
}

template <typename EventType, typename RecieverType>
void EventManager::Subscribe(RecieverType& receiver) {
	static_assert(Receivable<RecieverType, EventType>, "Reciever must implement Recieve() method.");

	void (RecieverType::*receive)(const EventType& event) = &RecieverType::Recieve;
    EventBase::FamilyType family = Event<EventType>::Family();

	std::shared_ptr<EventSignal>& signal = SignalFromFamily(family);
    EventCallbackWrapper<EventType>wrapper(std::bind(receive, &receiver, std::placeholders::_1));
    signal::CallBackId id = signal->Connect(wrapper);

    ReceiverBase& base = receiver;
    std::weak_ptr<EventSignal> pointer(signal);
    std::pair<std::weak_ptr<EventSignal>, signal::CallBackId> connection(pointer, id);
    base.ConnectionsList().insert(std::make_pair(family, connection));
}

template <typename EventType, typename RecieverType>
void EventManager::Unsubscribe(RecieverType& receiver) {
    ReceiverBase& base = receiver;
    auto connections = base.ConnectionsList();
    EventBase::FamilyType family = Event<EventType>::Family();

    auto found_connection = connections.find(family);
    if (found_connection == connections.end()) {
    	return;
    }

    std::weak_ptr<EventSignal> pointer = found_connection->second.first;
	signal::CallBackId id = found_connection->second.second;
	if (!pointer.expired()) {
		pointer.lock()->Disconnect(id);
	}

	connections.erase(family);
}

template <typename EventType>
void EventManager::Emit(const EventType& event) {
	std::shared_ptr<EventSignal>& signal = SignalFromFamily(Event<EventType>::Family());
	signal->Emit(&event);
}

template <typename EventType>
void EventManager::Emit(std::unique_ptr<EventType> event) {
	std::shared_ptr<EventSignal>& signal = SignalFromFamily(Event<EventType>::Family());
	signal->Emit(event.get());
}

template <typename EventType, typename... ArgTypes>
void EventManager::Emit(ArgTypes&&... args) {
	EventType event(std::forward<ArgTypes>(args)...);
	std::shared_ptr<EventSignal>& signal = SignalFromFamily(Event<EventType>::Family());
	signal->Emit(&event);
}

size_t EventManager::RecieversCount() const {
	size_t count = 0;
	for (std::shared_ptr<EventSignal> handler : handlers_) {
		if (handler) {
			count += handler->CallbackCount();
		}
	}
	return count;
}

std::shared_ptr<EventSignal>& EventManager::SignalFromFamily(EventBase::FamilyType family) {
	if (family >= handlers_.size()) {
		handlers_.resize(family + 1);
	}

	if (!handlers_[family]) {
		handlers_[family] = std::make_shared<EventSignal>();
	}

	return handlers_[family];
}

} // namespace ecs
