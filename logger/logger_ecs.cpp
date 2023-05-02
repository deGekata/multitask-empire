#include <logger/logger_ecs.hpp>
#include <logger/components_formatting.hpp>
#include <logger/logger.hpp>

#define ENTITY_ID_STR ("<" + std::to_string(event.entity_.GetId().GetIndex()) + ":" + std::to_string(event.entity_.GetId().GetGeneration()) + ">")

// define is required because we have to write the identical method on each component, that we want to log.
// So otherwise there would be huge amount of identical methods Recieve on every type of component
// If you see the better way how to implement this without macroses, please make your suggestion

#define BASE_COMP_ADD_RESPONSE(T_COMPONENT)                                                          \
void EcsBaseLogger::Recieve(const ecs::ComponentAddedEvent<T_COMPONENT>& event) {                                    \
    assert(event.entity_.IsValid());                                                                                  \
    \
    logger::Print(kInfo, "Entity{} gained {}[{}]\n", fmt::styled(ENTITY_ID_STR, fmt::fg(fmt::color(logger::kEcsSystemHex))), fmt::styled(#T_COMPONENT, fmt::fg(fmt::color(logger::kEcsComponentHex))), *(event.component_.Get())); \
}

#define BASE_COMP_REMOVE_RESPONSE(T_COMPONENT)                                         \
void EcsBaseLogger::Recieve(const ecs::ComponentRemovedEvent<T_COMPONENT>& event) {                    \
    assert(event.entity_.IsValid());                                                                    \
                                                                                                         \
    logger::Print(kInfo, "Entity{} lost {}[{}]\n", fmt::styled(ENTITY_ID_STR, fmt::fg(fmt::color(logger::kEcsSystemHex))), fmt::styled(#T_COMPONENT, fmt::fg(fmt::color(logger::kEcsComponentHex))), *(event.component_.Get())); \
}

// define to avoid copypaste
#define BASE_COMP_ALL_RESPONSE(T_COMPONENT) \
BASE_COMP_ADD_RESPONSE(T_COMPONENT)  \
BASE_COMP_REMOVE_RESPONSE(T_COMPONENT)

void EcsBaseLogger::Recieve(const ecs::EntityCreatedEvent& event) {
    assert(event.entity_.IsValid());
    logger::Print(kInfo, "Entity{} was created\n", fmt::styled(ENTITY_ID_STR, fmt::fg(fmt::color(logger::kEcsSystemHex))));
}

void EcsBaseLogger::Recieve(const ecs::EntityDestroyedEvent& event) {
    assert(event.entity_.IsValid());
    logger::Print(kInfo, "Entity{} was destroyed\n", fmt::styled(ENTITY_ID_STR, fmt::fg(fmt::color(logger::kEcsSystemHex))));
}

BASE_COMP_ALL_RESPONSE(PlayerTag)
BASE_COMP_ALL_RESPONSE(Position)
BASE_COMP_ALL_RESPONSE(Velocity)
BASE_COMP_ALL_RESPONSE(Acceleration)

void EcsFullLogger::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    events.Subscribe<ecs::EntityCreatedEvent, EcsFullLogger>(*this);
    events.Subscribe<ecs::EntityDestroyedEvent, EcsFullLogger>(*this);

    SubsribeComponent<PlayerTag>(this, entities, events);
    SubsribeComponent<Position>(this, entities, events);
    SubsribeComponent<Velocity>(this, entities, events);
    SubsribeComponent<Acceleration>(this, entities, events);
}

void EcsFullLogger::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}
