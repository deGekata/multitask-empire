#ifndef H_CURSES_EVENTS
#define H_CURSES_EVENTS

#include <ecs/event.hpp>

#include <components/battle_components.hpp>

struct ActiveCurseEvent : public ecs::Event<ActiveCurseEvent> {
    ActiveCurseEvent(ecs::Entity entity, ActiveCurseInfo curse) : entity_(entity), curse_(std::move(curse)) {
    }

    ecs::Entity entity_;
    ActiveCurseInfo curse_;
};

struct PassiveCurseEvent : public ecs::Event<PassiveCurseEvent> {
    PassiveCurseEvent(ecs::Entity entity, PassiveCurseInfo curse) : entity_(entity), curse_(std::move(curse)) {
    }

    ecs::Entity entity_;
    PassiveCurseInfo curse_;
};

#endif
