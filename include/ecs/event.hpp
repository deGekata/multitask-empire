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
    { object.Recieve(event) } -> std::same_as<void>;
};

using EventSignal = signal::Signal<void(const void*)>;

/**
 * @brief      This class describes the ECS Event base class
 *             holding family counter.
 */
class EventBase {
public:
    using FamilyType = uint64_t;

    EventBase();
    virtual ~EventBase();

protected:
    static FamilyType family_counter_;
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

    virtual ~Event() override {
    }

    static FamilyType Family() {
        static FamilyType family = family_counter_++;
        assert(family < kMaxEvents);
        return family;
    }
};

class EventTracker {
public:

    static void Reset();

    template<typename EventType>
    static void Track() {
        tracking_events_.set(Event<EventType>::Family());
    }

    template<typename EventType>
    static void UnTrack() {
        tracking_events_.reset(Event<EventType>::Family());
    }

    template<typename EventType>
    static bool IsTracking() {
        return tracking_events_.test(Event<EventType>::Family());
    }

private:
    static std::bitset<kMaxEvents> tracking_events_;
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
class Reciever : public ReceiverBase {
public:
    Reciever() : ReceiverBase() {
    }

    virtual ~Reciever() override {
    }
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
    virtual ~EventManager();

    template <typename EventType, typename RecieverType>
    void Subscribe(RecieverType& reciever) {
        static_assert(Receivable<RecieverType, EventType>, "Reciever must implement Recieve() method.");

        void (RecieverType::*receive)(const EventType& event) = &RecieverType::Recieve;
        EventBase::FamilyType family = Event<EventType>::Family();

        // Prepare subscriber info to insert into connections list
        std::shared_ptr<EventSignal>& signal = SignalFromFamily(family);
        EventCallbackWrapper<EventType> wrapper(std::bind(receive, &reciever, std::placeholders::_1));
        signal::CallBackId id = signal->Connect(wrapper);

        ReceiverBase& base = reciever;
        std::weak_ptr<EventSignal> pointer(signal);
        std::pair<std::weak_ptr<EventSignal>, signal::CallBackId> connection(pointer, id);
        base.ConnectionsList().insert(std::make_pair(family, connection));
    }

    template <typename EventType, typename RecieverType>
    void Unsubscribe(RecieverType& reciever) {
        ReceiverBase& base = reciever;
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

        if(EventTracker::IsTracking<EventType>()){
            logger::Print(kInfo, "Event{} was emited\n", fmt::styled("<" + logger::Type<EventType>() + ">", fmt::fg(fmt::rgb(0x33cccc))));
        }
    }

    template <typename EventType>
    void Emit(std::unique_ptr<EventType> event) {
        std::shared_ptr<EventSignal>& signal = SignalFromFamily(Event<EventType>::Family());
        signal->Emit(event.get());

        if(EventTracker::IsTracking<EventType>()){
            logger::Print(kInfo, "Event{} was emited\n", fmt::styled("<" + logger::Type<EventType>() + ">", fmt::fg(fmt::rgb(0x33cccc))));
        }
    }

    template <typename EventType, typename... ArgTypes>
    void Emit(ArgTypes&&... args) {
        EventType event(std::forward<ArgTypes>(args)...);
        std::shared_ptr<EventSignal>& signal = SignalFromFamily(Event<EventType>::Family());
        signal->Emit(&event);

        if(EventTracker::IsTracking<EventType>()){
            logger::Print(kInfo, "Event{} was emited\n", fmt::styled("<" + logger::Type<EventType>() + ">", fmt::fg(fmt::rgb(0x33cccc))));
        }
    }

    size_t RecieversCount() const;

private:
    std::shared_ptr<EventSignal>& SignalFromFamily(EventBase::FamilyType family);

private:
    std::vector<std::shared_ptr<EventSignal>> handlers_;
};

}  // namespace ecs

#endif  // ECS_EVENT_HPP
