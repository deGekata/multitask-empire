#include <logger/logger_ecs.hpp>
#include <logger/components_formatting.hpp>
#include <logger/logger.hpp>

#define ENTITY_ID_STR ("<" + std::to_string(event.entity_.GetId().GetIndex()) + ":" + std::to_string(event.entity_.GetId().GetGeneration()) + ">")

#define BASE_COMP_ADD_RESPONSE(T_COMPONENT)                                                          \
void EcsBaseLogger::Recieve(const ecs::ComponentAddedEvent<T_COMPONENT>& event) {                                    \
    assert(event.entity_.IsValid());                                                                                  \
    \
    logger::print(INFO, "Entity{} gained {}[{}]\n", fmt::styled(ENTITY_ID_STR, fmt::fg(fmt::color(0xEE51B1))), fmt::styled(#T_COMPONENT, fmt::fg(fmt::color(0xE3C515))), *(event.component_.Get())); \
}

#define BASE_COMP_EDIT_RESPONSE(T_COMPONENT)                                         \
void EcsBaseLogger::Recieve(const ecs::ComponentAccessedEvent<T_COMPONENT>& event) {                    \
    assert(event.entity_.IsValid());                                                                    \
                                                                                                         \
    logger::print(INFO, "Entity{} accessed {}[{}]\n", fmt::styled(ENTITY_ID_STR, fmt::fg(fmt::color(0xEE51B1))), fmt::styled(#T_COMPONENT, fmt::fg(fmt::color(0xE3C515))), *(event.component_.Get())); \
}

#define BASE_COMP_REMOVE_RESPONSE(T_COMPONENT)                                         \
void EcsBaseLogger::Recieve(const ecs::ComponentRemovedEvent<T_COMPONENT>& event) {                    \
    assert(event.entity_.IsValid());                                                                    \
                                                                                                         \
    logger::print(INFO, "Entity{} lost {}[{}]\n", fmt::styled(ENTITY_ID_STR, fmt::fg(fmt::color(0xEE51B1))), fmt::styled(#T_COMPONENT, fmt::fg(fmt::color(0xE3C515))), *(event.component_.Get())); \
}

#define BASE_COMP_ALL_RESPONSE(T_COMPONENT) \
BASE_COMP_ADD_RESPONSE(T_COMPONENT)  \
BASE_COMP_EDIT_RESPONSE(T_COMPONENT)  \
BASE_COMP_REMOVE_RESPONSE(T_COMPONENT)

void EcsBaseLogger::Recieve(const ecs::EntityCreatedEvent& event) {
    assert(event.entity_.IsValid());
    logger::print(INFO, "Entity{} was created\n", fmt::styled(ENTITY_ID_STR, fmt::fg(fmt::color(0xEE51B1))));
}

void EcsBaseLogger::Recieve(const ecs::EntityAccessedEvent& event) {
    assert(event.entity_.IsValid());
    logger::print(INFO, "Entity{} was accessed\n", fmt::styled(ENTITY_ID_STR, fmt::fg(fmt::color(0xEE51B1))));
}

void EcsBaseLogger::Recieve(const ecs::EntityDestroyedEvent& event) {
    assert(event.entity_.IsValid());
    logger::print(INFO, "Entity{} was destroyed\n", fmt::styled(ENTITY_ID_STR, fmt::fg(fmt::color(0xEE51B1))));
}

BASE_COMP_ALL_RESPONSE(PlayerTag)
BASE_COMP_ALL_RESPONSE(Position)
BASE_COMP_ALL_RESPONSE(Velocity)
BASE_COMP_ALL_RESPONSE(Acceleration)

#define COMP_SUBSCRIBE(T_COMPONENT)     \
{       \
    events.Subscribe<ecs::ComponentAddedEvent<T_COMPONENT>, EcsFullLogger>(*this); \
    events.Subscribe<ecs::ComponentAccessedEvent<T_COMPONENT>, EcsFullLogger>(*this); \
    events.Subscribe<ecs::ComponentRemovedEvent<T_COMPONENT>, EcsFullLogger>(*this); \
    ecs::Tracker::TrackComponentOnAdding<T_COMPONENT>();   \
    ecs::Tracker::TrackComponentOnAccess<T_COMPONENT>();      \
    ecs::Tracker::TrackComponentOnRemoving<T_COMPONENT>();   \
}

void EcsFullLogger::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<ecs::EntityCreatedEvent, EcsFullLogger>(*this);
    events.Subscribe<ecs::EntityAccessedEvent, EcsFullLogger>(*this);
    events.Subscribe<ecs::EntityDestroyedEvent, EcsFullLogger>(*this);

    COMP_SUBSCRIBE(PlayerTag)
    COMP_SUBSCRIBE(Position)
    COMP_SUBSCRIBE(Velocity)
    COMP_SUBSCRIBE(Acceleration)

    ecs::EventTracker::Track<ecs::ComponentAccessedEvent<Position>>();
}

void EcsFullLogger::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}
