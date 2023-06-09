/**
 * @defgroup   EVENT ECS Event
 *
 * @brief      This file implements ECS Event class.
 *
 * @details    Event is a mechanism used for inter-system
 *             communication is ECS.
 *
 * @author     Stanislav
 * @date       2023
 */

#ifndef ECS_EVENT_HPP
#define ECS_EVENT_HPP

// C sdtlib
#include <cstdint>
#include <cassert>

// C++ stdlib
#include <memory>

// Containers
#include <unordered_map>
#include <vector>

// This project
#include <signal/signal.hpp>
#include <utility/non_copiable.hpp>
#include <ecs/config.hpp>
#include <logger/logger.hpp>

namespace ecs {

template <typename Class, typename EventType>
concept Receivable = requires(Class object, const EventType& event) {
                         { object.Receive(event) } -> std::same_as<void>;
                     };

using EventSignal = signal::Signal<void(const void*)>;

/**
 * @brief      This class describes the ECS Event base class
 *             holding family counter.
 */
class EventBase {
public:
    using FamilyType = uint64_t;

    virtual ~EventBase() = default;

protected:
    static FamilyType family_counter;
};

/**
 * @brief      This class describes the main ECS Event class.
 *             All the custom event should be derived from this class.
 *
 * @tparam     Derived  Custom Event class.
 */
template <typename Derived>
class Event : public EventBase {
public:
    Event() : EventBase() {
    }

    static FamilyType Family() {
        static FamilyType family = family_counter++;
        assert(family < kMaxEvents);
        return family;
    }
};

class EventTrackingManager {
public:
    EventTrackingManager();

    EventTrackingManager(const EventTrackingManager& other) = default;
    ~EventTrackingManager() = default;

    template <typename EventType>
    void Track() {
        tracking_events_.set(Event<EventType>::Family());
    }

    template <typename EventType>
    void UnTrack() {
        tracking_events_.reset(Event<EventType>::Family());
    }

    template <typename EventType>
    bool IsTracking() {
        return tracking_events_.test(Event<EventType>::Family());
    }

private:
    std::bitset<kMaxEvents> tracking_events_;
};

/**
 * @brief      This class describes the ECS Receiver base class.
 *             Holds the main receiver logic.
 */
class ReceiverBase {
public:
    friend class EventManager;

    using ConnectionsInfo =
        std::unordered_map<EventBase::FamilyType, std::pair<std::weak_ptr<EventSignal>, signal::CallBackId>>;

    ReceiverBase();
    virtual ~ReceiverBase();

    size_t SignalsCount() const;

private:
    ConnectionsInfo ConnectionsList();

private:
    ConnectionsInfo connections_;
};

/**
 * @brief      This class describes the main ECS Receiver class
 *             to be derived from.
 *
 * @tparam     Derived  Custom Receiver class.
 */
template <typename Derived>
class Receiver : public ReceiverBase {
public:
    Receiver() : ReceiverBase() {
    }

    ~Receiver() override = default;
};

/**
 * @brief      This class describes the ECS Event Manager.
 *             Manages all the receiver - event communications.
 */
class EventManager : public utility::NonCopiable {
private:
    /**
     * @brief      This class describes an event callback wrapper.
     *
     * @tparam     EventType  Type of the event.
     */
    template <typename EventType>
    class EventCallbackWrapper {
    public:
        explicit EventCallbackWrapper(std::function<void(const EventType&)> callback) : callback_(callback) {
        }

        void operator()(const void* event) {
            auto event_arg = static_cast<const EventType*>(event);
            callback_(*event_arg);
        }

    private:
        std::function<void(const EventType&)> callback_;
    };

public:
    EventManager();
    virtual ~EventManager() = default;

    template <typename EventType, typename ReceiverType>
    void Subscribe(ReceiverType& receiver) {
        static_assert(Receivable<ReceiverType, EventType>, "Receiver must implement Receive() method.");

        void (ReceiverType::*receive)(const EventType& event) = &ReceiverType::Receive;
        EventBase::FamilyType family = Event<EventType>::Family();

        // Prepare subscriber info to insert into connections list
        std::shared_ptr<EventSignal>& signal = SignalFromFamily(family);
        EventCallbackWrapper<EventType> wrapper(std::bind(receive, &receiver, std::placeholders::_1));
        signal::CallBackId id = signal->Connect(wrapper);

        ReceiverBase& base = receiver;
        std::weak_ptr<EventSignal> pointer(signal);
        std::pair<std::weak_ptr<EventSignal>, signal::CallBackId> connection(pointer, id);
        base.ConnectionsList().insert(std::make_pair(family, connection));
    }

    template <typename EventType, typename ReceiverType>
    void Unsubscribe(ReceiverType& receiver) {
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
    void Emit(const EventType& event) {
        std::shared_ptr<EventSignal>& signal = SignalFromFamily(Event<EventType>::Family());
        signal->Emit(&event);

        if (tracker_.IsTracking<EventType>()) {
            logger::Print(kInfo, "Event{} was emited\n",
                          fmt::styled("<" + logger::Type<EventType>() + ">", fmt::fg(fmt::rgb(logger::kEcsEventHex))));
        }
    }

    template <typename EventType>
    void Emit(std::unique_ptr<EventType> event) {
        std::shared_ptr<EventSignal>& signal = SignalFromFamily(Event<EventType>::Family());
        signal->Emit(event.get());

        if (tracker_.IsTracking<EventType>()) {
            logger::Print(kInfo, "Event{} was emited\n",
                          fmt::styled("<" + logger::Type<EventType>() + ">", fmt::fg(fmt::rgb(logger::kEcsEventHex))));
        }
    }

    template <typename EventType, typename... ArgTypes>
    void Emit(ArgTypes&&... args) {
        EventType event(std::forward<ArgTypes>(args)...);
        std::shared_ptr<EventSignal>& signal = SignalFromFamily(Event<EventType>::Family());
        signal->Emit(&event);

        if (tracker_.IsTracking<EventType>()) {
            logger::Print(kInfo, "Event{} was emited\n",
                          fmt::styled("<" + logger::Type<EventType>() + ">", fmt::fg(fmt::rgb(logger::kEcsEventHex))));
        }
    }

    size_t ReceiversCount() const;

    EventTrackingManager& Tracker();

private:
    std::shared_ptr<EventSignal>& SignalFromFamily(EventBase::FamilyType family);

private:
    std::vector<std::shared_ptr<EventSignal>> handlers_;

    EventTrackingManager tracker_;
};

}  // namespace ecs

#endif  // ECS_EVENT_HPP
