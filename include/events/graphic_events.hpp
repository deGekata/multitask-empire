#ifndef H_GRAPHIC_EVENTS
#define H_GRAPHIC_EVENTS

#include <ecs/event.hpp>
#include <graphics/aWindow.hpp>

struct WindowInitiatedEvent : public ecs::Event<WindowInitiatedEvent> {
    explicit WindowInitiatedEvent(igraphicslib::Window* p_window) : p_window_(p_window) {
    }

    ~WindowInitiatedEvent() override = default;

    igraphicslib::Window* p_window_;
};

struct WindowClosedEvent : public ecs::Event<WindowClosedEvent> {
    explicit WindowClosedEvent(igraphicslib::Window* p_window) : p_window_(p_window) {
    }

    ~WindowClosedEvent() override = default;

    igraphicslib::Window* p_window_;
};


#endif  // H_GRAPHIC_EVENTS
