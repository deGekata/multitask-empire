#include <ecs/event.hpp>

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

ReceiverBase::ConnectionsInfo ReceiverBase::ConnectionsList() {
	return connections_;
}

EventManager::EventManager()
	: utility::NonCopiable() {
}

EventManager::~EventManager() {
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
