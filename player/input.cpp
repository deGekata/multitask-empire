#include <player/input.hpp>
#include <ecs/entity.hpp>
#include <components/player_components.hpp>

#include <events/player_events.hpp>

void KeyboardInputSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<WindowInitiatedEvent, KeyboardInputSystem>(*this);
    events.Subscribe<WindowClosedEvent, KeyboardInputSystem>(*this);

    cur_key_ = igraphicslib::KeyboardKey::Unknown;
}

void KeyboardInputSystem::Update(ecs::EntityManager&, ecs::EventManager& events, ecs::TimeDelta) {

    for (auto event = events_queue_.front(); !events_queue_.empty(); events_queue_.pop_front()) {
        if (event.type == igraphicslib::EventType::KeyPressed) {
                            
            if (event.ked.key == cur_key_) {
                continue;
            }

            cur_key_ = event.ked.key;
            events.Emit<KeyPressedEvent>(event.ked);
        } else if (event.type == igraphicslib::EventType::KeyReleased) {

            events.Emit<KeyReleasedEvent>(event.ked);
            cur_key_ = igraphicslib::KeyboardKey::Unknown;
        }
    }
}

void KeyboardInputSystem::Receive(const WindowInitiatedEvent& event) {
    if (!pooling_lock_.try_lock()) {
        std::cout << "WARNING, unable to link window to keyboard input system, because pooling is online\n";
        return;
    }

    root_window_ = event.p_window_;
    pooling_lock_.unlock();
}

void KeyboardInputSystem::Receive(const WindowClosedEvent& event) {
    if (!pooling_lock_.try_lock()) {
        std::cout << "WARNING, unable to link window to keyboard input system, because pooling is online\n";
        return;
    }

    if (root_window_ == event.p_window_) {
        root_window_ = nullptr;
    }

    pooling_lock_.unlock();
}

void KeyboardInputSystem::Pool() {
    std::lock_guard<std::mutex> guard(pooling_lock_);
    assert(root_window_);

    while (!is_request_to_stop_pooling_.load()) {
        igraphicslib::Event event;
        if (root_window_->PollEvent(event)) {
            if (event.type == igraphicslib::EventType::KeyPressed ||
                event.type == igraphicslib::EventType::KeyReleased) {

                events_queue_.push_back(event);
            }
        }
    }
}

void KeyboardInputSystem::StopPool() {
    is_request_to_stop_pooling_.store(true);

    // if we able to lock pooling_lock_, it means that pooling has stopped
    std::lock_guard<std::mutex> guard(pooling_lock_);
}
