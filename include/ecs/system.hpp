#ifndef H_SYSTEM
#define H_SYSTEM

#include <map>

#include <ecs/config.hpp>
#include <ecs/entity.hpp>
#include <ecs/event.hpp>
#include <iostream>
#include <logger/logger.hpp>

namespace ecs {
class BaseSystem {
public:
    using Family = uint64_t;

    virtual ~BaseSystem();

    /**
     * Called once all Systems have been added to the SystemManager.
     *
     * Typically used to set up event handlers.
     */
    virtual void Configure(EntityManager& entities, EventManager& events);

    /**
     * Apply System behavior.
     *
     * Called every game step.
     */
    virtual void Update(EntityManager& entities, EventManager& events, TimeDelta dt) = 0;

protected:
    static Family family_counter_;
};

/**
 * Use this class when implementing Systems.
 *
 * struct MovementSystem : public System<MovementSystem> {
 *   void update(EntityManager &entities, EventManager &events, TimeDelta dt) {
 *     // Do stuff to/with entities...
 *   }
 * }
 */

template <typename Derived>
class System : public BaseSystem {
public:
    virtual ~System() override {
    }

private:
    friend class SystemManager;

    static Family GetFamily() {
        static Family family = family_counter_++;

        return family;
    }
};

class SystemManager {
public:
    SystemManager(EntityManager& entity_manager, EventManager& event_manager);
    ~SystemManager();

    /**
     * Add a System to the SystemManager.
     *
     * Must be called before Systems can be used.
     *
     * eg.
     * MovementSystem* movement = new MovementSystem();
     * system.Add(movement);
     */
    template <typename System>
    void Add(System* system) {
        systems_.insert(std::make_pair(System::GetFamily(), system));
        log<INFO>("System {} added\n", type(*system));
    }

    /**
     * Add a System to the SystemManager.
     *
     * Must be called before Systems can be used.
     *
     * eg.
     * auto* movement = system.Add<MovementSystem>();
     */
    template <typename System, typename... Args>
    System* Add(Args&&... args) {
        System* s = new System(std::forward<Args>(args)...);
        Add(s);
        return s;
    }

    /**
     * Retrieve the registered System instance, if any.
     *
     *   CollisionSystem* collisions = systems.system<CollisionSystem>();
     *
     * @return System* instance or nullptr.
     */

    template <typename System>
    System* GetSystem() {
        auto it = systems_.find(System::GetFamily());

        if (it == systems_.end()) {
            return nullptr;
        }

        return it->second;
    }

    /**
     * Call the System::update() method for a registered system.
     */

    template <typename System>
    void Update(TimeDelta dt) {
        assert(is_initialized_ && "SystemManager::configure() not called");

        auto* s = GetSystem<System>();
        assert(s);

        s->Update(entity_manager_, event_manager_, dt);
    }

    /**
     * Call System::update() on all registered systems.
     */
    void UpdateAll(TimeDelta dt);

    /**
     * Configure the system. Call after adding all Systems.
     *
     * This is typically used to set up event handlers.
     */
    void Configure();

private:
    bool is_initialized_;

    EntityManager& entity_manager_; 
    EventManager&  event_manager_;   

    std::map<BaseSystem::Family, BaseSystem*> systems_;
};

};  // namespace ecs

#endif
