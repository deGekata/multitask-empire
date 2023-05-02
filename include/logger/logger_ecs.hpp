#ifndef LOGGER_ECS_H
#define LOGGER_ECS_H

#include <ecs/system.hpp>
#include <ecs/entity.hpp>
#include <components/player_components.hpp>
#include <components/movement_components.hpp>

// TODO: DLOG implementing + save data into file

class EcsBaseLogger : public ecs::System<EcsBaseLogger>, public ecs::ReceiverBase{
public:
    // entity
    virtual void Recieve(const ecs::EntityCreatedEvent& event);
    virtual void Recieve(const ecs::EntityAccessedEvent& event);
    virtual void Recieve(const ecs::EntityDestroyedEvent& event);
    
    // player_components.hpp
    virtual void Recieve(const ecs::ComponentAddedEvent<PlayerTag>& event);
    virtual void Recieve(const ecs::ComponentRemovedEvent<PlayerTag>& event);
    
    // // movement_components.hpp
    virtual void Recieve(const ecs::ComponentAddedEvent<Position>& event);
    virtual void Recieve(const ecs::ComponentRemovedEvent<Position>& event);
    
    virtual void Recieve(const ecs::ComponentAddedEvent<Velocity>& event);
    virtual void Recieve(const ecs::ComponentRemovedEvent<Velocity>& event);
    
    virtual void Recieve(const ecs::ComponentAddedEvent<Acceleration>& event);
    virtual void Recieve(const ecs::ComponentRemovedEvent<Acceleration>& event);
};

class EcsFullLogger : public EcsBaseLogger{
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;
protected:
    template<typename Component, class LoggerSystem>
    void SubsribeComponent(LoggerSystem* p_system, ecs::EntityManager& entities, ecs::EventManager& events) {
        events.Subscribe<ecs::ComponentAddedEvent<Component>, LoggerSystem>(*p_system);
        events.Subscribe<ecs::ComponentRemovedEvent<Component>, LoggerSystem>(*p_system);
        entities.Tracker().TrackComponentOnAdding<Component>();
        entities.Tracker().TrackComponentOnRemoving<Component>();
    }
};

#endif // LOGGER_ECS_H
