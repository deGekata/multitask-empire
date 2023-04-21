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

// C++ stdlib
#include <memory>

// Containers
#include <unordered_map>
#include <vector>

// This project
#include "signal/signal.hpp"
#include "utility/non_copiable.hpp"

namespace ecs {

template <typename Class, typename EventType>
concept Receivable = requires(Class object, const EventType& event) {
    { object.Recieve(event) } -> std::same_as<void>;
};

using EventSignal = signal::Signal<void (const void*)>;

class EventBase {
public:
    using FamilyType = uint64_t;

    EventBase();
    virtual ~EventBase();

protected:
    static FamilyType family_counter_;
};

template <typename Derived>
class Event
    : public EventBase {
public:
    Event()
        : EventBase() {
    }

    virtual ~Event() override {
    }

    static FamilyType Family() {
        static FamilyType family = family_counter_++;
        return family;
    }
};

class ReceiverBase {
public:
    friend class EventManager;

    ReceiverBase();
    virtual ~ReceiverBase();

    size_t SignalsCount() const;

private:
    auto ConnectionsList();

private:
    std::unordered_map<EventBase::FamilyType, std::pair<std::weak_ptr<EventSignal>, signal::CallBackId>> connections_;
};

template <typename Derived>
class Reciever
    : public ReceiverBase {
public:
    Reciever()
        : ReceiverBase() {
    }

    virtual ~Reciever() override {
    }
};

class EventManager
    : public utility::NonCopiable {
private:
    /**
     * @brief      This class describes an event callback wrapper.
     *
     * @tparam     EventType  Type of the event.
     */
    template <typename EventType>
    class EventCallbackWrapper {
    public:
        explicit EventCallbackWrapper(std::function<void (const EventType&)> callback)
            : callback_(callback) {
        }

        void operator()(const void* event) {
            auto event_arg = static_cast<const EventType*>(event);
            callback_(*event_arg);
        }

    private:
        std::function<void (const EventType&)> callback_;
    };

public:
    EventManager();
    virtual ~EventManager();

    template <typename EventType, typename RecieverType>
    void Subscribe(RecieverType& reciever);

    template <typename EventType, typename RecieverType>
    void Unsubscribe(RecieverType& reciever);

    template <typename EventType>
    void Emit(const EventType& event);

    template <typename EventType>
    void Emit(std::unique_ptr<EventType> event);

    template <typename EventType, typename... ArgTypes>
    void Emit(ArgTypes&&... args);

    size_t RecieversCount() const;

private:
    std::shared_ptr<EventSignal>& SignalFromFamily(EventBase::FamilyType family);

private:
    std::vector<std::shared_ptr<EventSignal>> handlers_;
};

} // namespace ecs

#endif // ECS_EVENT_HPP
