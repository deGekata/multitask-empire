#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include <deque>
#include <mutex>
#include <atomic>

#include <ecs/system.hpp>
#include <graphics/aWindow.hpp>
#include <events/graphic_events.hpp>

/// @brief values of key from igraphics Keyboard Event should be in range [0, 200) for the fast mapping
const size_t MAX_N_KEYS = 200;

// class BaseInputSystem {
// public:
//     virtual 
// };

/**
 * @brief the system will track all events coming from the keyboard on the last initiated window. 
 * The system receives events using the method of the graphics library method igraphics::Window::Pollevent, 
 * if the event is a keyboard event, it matches the pressed key combination to the player's command and emits the corresponding event.
 * System has to work on the additional thread, so if you want to use it, just create std::thread on method Pool
 * 
 */
class KeyboardInputSystem : public ecs::System<KeyboardInputSystem>, public ecs::Reciever<KeyboardInputSystem> {
public:
    
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    /// @brief these methods mustn't be called during pooling
    void Recieve(const WindowInitiatedEvent& event);
    void Recieve(const WindowClosedEvent& event);

    /**
     * @return true, if pooling began successfuly. false, if root_window_ wasn't initiated.
     */
    void Pool();

    /// @brief makes request to stop pooling and waits until pool would really stop
    void StopPool();
private:
    igraphicslib::Window*           root_window_;
    std::deque<igraphicslib::Event> events_queue_;

    // synchronization primitives
    std::mutex                      pooling_lock_;
    std::mutex                      system_members_access_lock_;
    std::atomic<bool>               is_request_to_stop_pooling_;
    std::atomic<bool>               is_pooling_is_stopped_;                
};

#endif // INPUT_SYSTEM_HPP
