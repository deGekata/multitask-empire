#include <ecs/event.hpp>

namespace ecs {

EventBase::FamilyType EventBase::family_counter = 0;

ReceiverBase::ReceiverBase() : connections_() {
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

EventManager::EventManager() : utility::NonCopiable(), tracker_() {
}

size_t EventManager::ReceiversCount() const {
    size_t count = 0;
    for (std::shared_ptr<EventSignal> handler : handlers_) {
        if (handler) {
            count += handler->CallbackCount();
        }
    }
    return count;
}

EventTrackingManager& EventManager::Tracker() {
    return tracker_;
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

EventTrackingManager::EventTrackingManager() {
    tracking_events_.reset();
}

}  // namespace ecs
