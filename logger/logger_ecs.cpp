#include <logger/logger_ecs.hpp>
#include <logger/components_formatting.hpp>
#include <logger/logger.hpp>
#include <iostream>

#define BASE_COMP_ADD_RESPONSE(T_COMPONENT)                                                          \
void EcsBaseLogger::Recieve(const ecs::ComponentAddedEvent<T_COMPONENT>& event) {                                    \
    assert(event.entity_.IsValid());                                                                                  \
    \
    log<INFO>("Entity<{}> received {}({})\n", event.entity_.GetId().GetId(), #T_COMPONENT, *(event.component_.Get())); \
}

#define BASE_COMP_REMOVE_RESPONSE(T_COMPONENT)                                         \
void EcsBaseLogger::Recieve(const ecs::ComponentRemovedEvent<T_COMPONENT>& event) {                    \
    assert(event.entity_.IsValid());                                                                    \
                                                                                                         \
    log<INFO>("Entity<{}> lost {}({})\n", event.entity_.GetId().GetId(), #T_COMPONENT, *(event.component_.Get())); \
}

void EcsBaseLogger::Recieve(const ecs::EntityCreatedEvent& event) {
    assert(event.entity_.IsValid());
    log<INFO>("Entity<{}> was created\n", event.entity_.GetId().GetId());
}

void EcsBaseLogger::Recieve(const ecs::EntityDestroyedEvent& event) {
    assert(event.entity_.IsValid());
    log<INFO>("Entity<{}> was destroyed\n", event.entity_.GetId().GetId());
}

BASE_COMP_ADD_RESPONSE(PlayerTag)
BASE_COMP_REMOVE_RESPONSE(PlayerTag)

BASE_COMP_ADD_RESPONSE(Position)
BASE_COMP_REMOVE_RESPONSE(Position)
BASE_COMP_ADD_RESPONSE(Velocity)
BASE_COMP_REMOVE_RESPONSE(Velocity)
BASE_COMP_ADD_RESPONSE(Acceleration)
BASE_COMP_REMOVE_RESPONSE(Acceleration)

void EcsFullLogger::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<ecs::EntityCreatedEvent, EcsFullLogger>(*this);
    events.Subscribe<ecs::EntityDestroyedEvent, EcsFullLogger>(*this);
    events.Subscribe<ecs::ComponentAddedEvent<PlayerTag>, EcsFullLogger>(*this);
    events.Subscribe<ecs::ComponentRemovedEvent<PlayerTag>, EcsFullLogger>(*this);

    events.Subscribe<ecs::ComponentAddedEvent<Position>, EcsFullLogger>(*this);
    events.Subscribe<ecs::ComponentRemovedEvent<Position>, EcsFullLogger>(*this);

    events.Subscribe<ecs::ComponentAddedEvent<Velocity>, EcsFullLogger>(*this);
    events.Subscribe<ecs::ComponentRemovedEvent<Velocity>, EcsFullLogger>(*this);
    
    events.Subscribe<ecs::ComponentAddedEvent<Acceleration>, EcsFullLogger>(*this);
    events.Subscribe<ecs::ComponentRemovedEvent<Acceleration>, EcsFullLogger>(*this);
}

void EcsFullLogger::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}
