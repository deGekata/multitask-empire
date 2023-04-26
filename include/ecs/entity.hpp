#ifndef H_ENTITY
#define H_ENTITY

#include <cstdint>

#include <algorithm>
#include <bitset>
#include <functional>
#include <tuple>
#include <type_traits>
#include <vector>

#include <ecs/event.hpp>
#include <ecs/pool.hpp>
#include <ecs/config.hpp>

namespace ecs {

struct EntityManager;

template <typename Component>
class ComponentHandle;

class Entity {
public:
    class Id {
    public:
        explicit Id(const uint64_t id = 0);
        Id(const uint32_t index, const uint32_t generation);

        uint64_t GetId() const;

        bool operator==(const Id& other) const;
        bool operator!=(const Id& other) const;
        bool operator<(const Id& other) const;

        uint32_t GetIndex() const;
        uint32_t GetGeneration() const;

    private:
        uint64_t id_;
    };

    static const Id INVALID_ID;

    Entity();
    Entity(const Entity& other) = default;
    Entity& operator=(const Entity& other) = default;

    Entity(EntityManager* manager, Id id);

    bool operator==(const Entity& other) const;
    bool operator!=(const Entity& other) const;
    bool operator<(const Entity& other) const;

    bool IsValid() const;
    void Invalidate();

    Id GetId() const;

public:
    template <typename Component, typename... Args>
    ComponentHandle<Component> Assign(Args&&... args);

    template <typename Component>
    ComponentHandle<Component> AssignFromCopy(const Component& component);

    template <typename Component, typename... Args>
    ComponentHandle<Component> Replace(Args&&... args);

    template <typename Component>
    void Remove();

    template <typename Component>
    ComponentHandle<Component> GetComponent();

    template <typename... Components>
    std::tuple<ComponentHandle<Components>...> GetComponents();

    template <typename Component>
    bool HasComponent();

    /**
     * Destroy and invalidate this Entity.
     */
    void Destroy();

    ComponentMask GetComponentMask() const;

private:
    EntityManager* manager_;
    Id id_;
};

/**
 * Base component class, only used for insertion into collections.
 *
 * Family is used for registration.
 */
struct BaseComponent {
public:
    using Family = uint64_t;

    [[noreturn]] void operator delete(void* p);
    [[noreturn]] void operator delete[](void* p);

protected:
    static Family family_counter_;
};

/**
 * Component implementations should inherit from this.
 *
 * Components MUST provide a no-argument constructor.
 * Components SHOULD provide convenience constructors for initializing on assignment to an Entity::Id.
 *
 * This is a struct to imply that components should be data-only.
 */

template <typename Derived>
struct Component : public BaseComponent {
public:
    using Handle = ComponentHandle<Derived>;
    using ConstHandle = ComponentHandle<const Derived>;

private:
    friend struct EntityManager;
    /// Used internally for registration.
    static Family GetFamily() {
        static Family family = family_counter_++;
        assert(family < MAX_COMPONENTS);
        return family;
    }
};

/**
 * A ComponentHandle<C> is a wrapper around an instance of a component.
 *
 * It provides safe access to components. The handle will be invalidated under
 * the following conditions:
 *
 * - If a component is removed from its host entity.
 * - If its host entity is destroyed.
 */

template <typename Component>
class ComponentHandle {
public:
    ComponentHandle() : manager_(nullptr) {
    }

    bool IsValid() const;

    Component* operator->();
    const Component* operator->() const;

    Component& operator*();
    const Component& operator*() const;

    Component* Get();
    const Component* Get() const;

    void Remove();

    Entity GetEntity();

private:
    friend struct EntityManager;

    ComponentHandle(EntityManager* manager, Entity::Id id) : manager_(manager), id_(id) {
    }

    EntityManager* manager_;
    Entity::Id id_;
};

struct BaseComponentHelper {
    virtual ~BaseComponentHelper() {
    }
    virtual void RemoveComponent(Entity enity) = 0;
    virtual void CopyComponentTo(Entity from, Entity to) = 0;
};

template <typename Component>
struct ComponentHelper : public BaseComponentHelper {
    void RemoveComponent(Entity entity) override {
        entity.Remove<Component>();
    }

    void CopyComponentTo(Entity from, Entity to) override {
        to.AssignFromCopy(*from.GetComponent<Component>().Get());
    }
};

/**
 * Emitted when an entity is added to the system.
 */
struct EntityCreatedEvent : public Event<EntityCreatedEvent> {
    explicit EntityCreatedEvent(Entity entity) : entity_(entity) {
    }
    virtual ~EntityCreatedEvent() override;

    Entity entity_;
};

/**
 * Called just prior to an entity being destroyed.
 */
struct EntityDestroyedEvent : public Event<EntityDestroyedEvent> {
    explicit EntityDestroyedEvent(Entity entity) : entity_(entity) {
    }
    virtual ~EntityDestroyedEvent() override;

    Entity entity_;
};

template <typename C>
struct ComponentAddedEvent : public Event<ComponentAddedEvent<C>> {
    ComponentAddedEvent(Entity entity, ComponentHandle<C> component) : entity_(entity), component_(component) {
    }

    Entity entity_;
    ComponentHandle<C> component_;
};

/**
 * Emitted when any component is removed from an entity.
 */
template <typename C>
struct ComponentRemovedEvent : public Event<ComponentRemovedEvent<C>> {
    ComponentRemovedEvent(Entity entity, ComponentHandle<C> component) : entity_(entity), component_(component) {
    }

    Entity entity_;
    ComponentHandle<C> component_;
};

struct EntityManager {
public:
    explicit EntityManager(EventManager& event_manager);
    virtual ~EntityManager();

    /// An iterator over a view of the entities in an EntityManager.
    /// If ShowAll is true it will iterate over all valid entities and will ignore the entity mask.
    template <typename Delegate, bool ShowAll = false>
    class ViewIterator {
    public:
        using iterator_category = std::input_iterator_tag;
        using value_type = Entity::Id;

        Delegate& operator++() {
            ++index_;
            Next();
            return *static_cast<Delegate*>(this);
        }

        bool operator!=(const Delegate& rhs) const {
            return index_ != rhs.index_;
        }

        Entity operator*() {
            return Entity{manager_, manager_->GetId(index_)};
        }

        const Entity operator*() const {
            return Entity{manager_, manager_->GetId(index_)};
        }

    protected:
        ViewIterator(EntityManager* manager, uint32_t index)
            : manager_(manager), index_(index), capacity_(manager_->GetCapacity()) {
            if (ShowAll) {
                std::sort(manager_->free_list_.begin(), manager_->free_list_.end());
            }
        }

        ViewIterator(EntityManager* manager, const ComponentMask mask, uint32_t index)
            : manager_(manager), mask_(mask), index_(index), capacity_(manager_->GetCapacity()) {
            if (ShowAll) {
                std::sort(manager_->free_list_.begin(), manager_->free_list_.end());
            }
        }

        void Next() {
            while ((index_ < capacity_) && !Predicate()) {
                index_++;
            }
        }

        bool Predicate() {
            return ValidateIndex() || ((manager_->entity_component_mask_[index_] & mask_) == mask_);
        }

        bool ValidateIndex() {
            if (ShowAll == false) {
                return false;
            }

            static uint64_t free_cursor = 0;
            if ((free_cursor < manager_->free_list_.size()) && (manager_->free_list_[free_cursor] == index_)) {
                free_cursor++;

                return false;
            }

            return true;
        }

        EntityManager* manager_;
        ComponentMask mask_;

        uint32_t index_;

        uint64_t capacity_;
    };

    template <bool ShowAll>
    class BaseViewer {
    public:
        class Iterator : public ViewIterator<Iterator, ShowAll> {
        public:
            Iterator(EntityManager* manager, const ComponentMask mask, uint32_t index)
                : ViewIterator<Iterator, ShowAll>(manager, mask, index) {
                ViewIterator<Iterator, ShowAll>::Next();
            }
        };

        Iterator begin() {
            return Iterator{manager_, mask_, 0};
        }

        Iterator end() {
            return Iterator{manager_, mask_, static_cast<uint32_t>(manager_->GetCapacity())};
        }

        const Iterator begin() const {
            return Iterator{manager_, mask_, 0};
        }

        const Iterator end() const {
            return Iterator{manager_, mask_, static_cast<uint32_t>(manager_->GetCapacity())};
        }

    private:
        friend struct EntityManager;

        explicit BaseViewer(EntityManager* manager) : manager_(manager) {
            mask_.set();
        }

        BaseViewer(EntityManager* manager, ComponentMask mask) : manager_(manager), mask_(mask) {
        }

        EntityManager* manager_;
        ComponentMask mask_;
    };

    template <bool ShowAll, typename... Components>
    class TypedViewer : public BaseViewer<ShowAll> {
    public:
        using FunctionT = std::function<void(Entity, Components&...)>;

        void Each(FunctionT f) {
            for (auto it : *this) {
                f(it, *(it.template GetComponent<Components>().Get())...);
            }
        }

    private:
        friend struct EntityManager;

        explicit TypedViewer(EntityManager* manager) : BaseViewer<ShowAll>(manager) {
        }

        TypedViewer(EntityManager* manager, ComponentMask mask) : BaseViewer<ShowAll>(manager, mask) {
        }
    };

    void Reset();

    uint64_t GetCapacity() const;
    bool IsValid(const Entity::Id& id) const;

    Entity Create();
    Entity CreateFromCopy(const Entity original);

    void Destroy(const Entity::Id entity);

    // Retrieve the component family for a type.
    template <typename C>
    static BaseComponent::Family Family() {
        return Component<typename std::remove_const<C>::type>::GetFamily();
    }

    BaseViewer<true> GetAllEntities();
    Entity GetEntity(Entity::Id id);

    Entity::Id GetId(const uint32_t idx);

    /**
     * Assign a Component to an Entity::Id, passing through Component constructor arguments.
     *
     *     Position &position = em.assign<Position>(e, x, y);
     *
     * @returns Smart pointer to newly created component.
     */
    template <typename Component, typename... Args>
    ComponentHandle<Component> Assign(Entity::Id id, Args&&... args) {
        AssertId(id);

        BaseComponent::Family family = Family<Component>();
        assert(entity_component_mask_[id.GetIndex()].test(family) == false);

        auto* pool = AccomodateComponent<Component>();
        ::new (pool->GetElement(id.GetIndex())) Component(std::forward<Args>(args)...);

        entity_component_mask_[id.GetIndex()].set(family);

        ComponentHandle<Component> component{this, id};
        event_manager_.Emit<ComponentAddedEvent<Component>>(Entity{this, id}, component);
        return component;
    }

    /**
     * Remove a Component from an Entity::Id
     */

    template <typename Component>
    void Remove(Entity::Id id) {
        AssertId(id);

        BaseComponent::Family family = Family<Component>();

        auto* pool = component_pools_[family];
        ComponentHandle<Component> component{this, id};
        event_manager_.Emit<ComponentRemovedEvent<Component>>(Entity{this, id}, component);

        entity_component_mask_[id.GetIndex()].reset(family);
        pool->DestroyElement(id.GetIndex());
    }

    /**
     * Check if an Entity has a component.
     */
    template <typename Component>
    bool HasComponent(Entity::Id id) const {
        AssertId(id);

        BaseComponent::Family family = Family<Component>();
        if (family >= component_pools_.size()) {
            return false;
        }

        BasePool* pool = component_pools_[family];
        if ((pool == nullptr) || (entity_component_mask_[id.GetIndex()][family] == false)) {
            return false;
        }

        return true;
    }

    /**
     * Retrieve a Component assigned to an Entity::Id.
     *
     * @returns Pointer to an instance of C, or nullptr if the Entity::Id does not have that Component.
     */
    template <typename Component>
    ComponentHandle<Component> GetComponent(Entity::Id id) {
        if (HasComponent<Component>(id)) {
            return ComponentHandle<Component>(this, id);
        }

        return ComponentHandle<Component>();
    }

    template <typename... Components>
    std::tuple<ComponentHandle<Components>...> GetComponents(Entity::Id id) {
        return std::make_tuple(GetComponent<Components>(id)...);
    }

    /**
     * Find Entities that have all of the specified Components.
     *
     * @code
     * for (Entity entity : entity_manager.entities_with_components<Position, Direction>()) {
     *   ComponentHandle<Position> position = entity.component<Position>();
     *   ComponentHandle<Direction> direction = entity.component<Direction>();
     *
     *   ...
     * }
     * @endcode
     */

    template <typename... Components>
    TypedViewer<false, Components...> GetEntitiesWithComponents() {
        auto mask = GetComponentMask<Components...>();

        return TypedViewer<false, Components...>(this, mask);
    }

    template <typename T>
    struct identity {
        typedef T type;
    };

    template <typename... Components>
    void Each(typename identity<std::function<void(Entity entity, Components&...)>>::type f) {
        GetEntitiesWithComponents<Components...>().Each(std::move(f));
    }

private:
    friend class Entity;
    template <typename C>
    friend class ComponentHandle;

    void AssertId(const Entity::Id entity) const;

    template <typename Component>
    Component* GetComponentPtr(Entity::Id id) {
        AssertId(id);

        BasePool* pool = component_pools_[Family<Component>()];
        assert(pool);

        return static_cast<Component*>(pool->GetElement(id.GetIndex()));
    }

    // Return mask of current components of entity with id id
    ComponentMask GetComponentMask(Entity::Id id) const {
        AssertId(id);

        return entity_component_mask_[id.GetIndex()];
    }

    // Return mask that corresponds to entity with component Component
    template <typename Component>
    ComponentMask GetComponentMask() {
        ComponentMask mask;

        mask.set(Family<Component>());
        return mask;
    }

    // Return mask that corresponds to entity with components C1, C2...
    template <typename C1, typename C2, typename... Components>
    ComponentMask GetComponentMask() {
        return GetComponentMask<C1>() | GetComponentMask<C2, Components...>();
    }

    template <typename C>
    ComponentMask GetComponentMask(const ComponentHandle<C>&) {
        return GetComponentMask<C>();
    }

    template <typename C1, typename... Components>
    ComponentMask GetComponentMask(const ComponentHandle<C1>&, const ComponentHandle<Components>&...) {
        return GetComponentMask<C1, Components...>();
    }

    void AccomodateEntity(const uint32_t index);

    template <typename Component>
    Pool<Component>* AccomodateComponent() {
        BaseComponent::Family family = Family<Component>();

        if (component_pools_.size() <= family) {
            component_pools_.resize(family + 1, nullptr);
        }
        if (component_pools_[family] == nullptr) {
            auto* pool = new Pool<Component>();
            pool->Resize(index_counter_);

            component_pools_[family] = pool;
        }

        if (component_helpers_.size() <= family) {
            component_helpers_.resize(family + 1, nullptr);
        }
        if (component_helpers_[family] == nullptr) {
            auto* helper = new ComponentHelper<Component>();

            component_helpers_[family] = helper;
        }

        return static_cast<Pool<Component>*>(component_pools_[family]);
    }

    uint32_t index_counter_;

    EventManager& event_manager_;
    // Each element in component_pools_ corresponds to a Pool for a Component.
    // The index into the vector is the Component::family().
    std::vector<BasePool*> component_pools_;

    // Each element in component_helpers_ corresponds to a ComponentHelper for a Component type.
    // The index into the vector is the Component::family().
    std::vector<BaseComponentHelper*> component_helpers_;

    // Bitmask of components associated with each entity. Index into the vector is
    // the Entity::Id.
    std::vector<ComponentMask> entity_component_mask_;

    // Vector of entity generation numbers. Incremented each time an entity is
    // destroyed
    std::vector<uint32_t> entity_generations_;

    // List of available entity slots.
    std::vector<uint32_t> free_list_;
};

template <typename Component, typename... Args>
ComponentHandle<Component> Entity::Assign(Args&&... args) {
    assert(IsValid());

    return manager_->Assign<Component>(id_, std::forward<Args>(args)...);
}

template <typename Component>
ComponentHandle<Component> Entity::AssignFromCopy(const Component& component) {
    assert(IsValid());

    return manager_->Assign<Component>(id_, std::forward<const Component&>(component));
}

template <typename Component, typename... Args>
ComponentHandle<Component> Entity::Replace(Args&&... args) {
    assert(IsValid());

    ComponentHandle<Component> handle = GetComponent<Component>();
    if (handle) {
        *(handle.Get()) = Component(std::forward<Args>(args)...);
    } else {
        handle = manager_->Assign<Component>(id_, std::forward<Args>(args)...);
    }

    return handle;
}

template <typename Component>
void Entity::Remove() {
    assert(IsValid() && HasComponent<Component>());
    manager_->Remove<Component>(id_);
}

template <typename Component>
ComponentHandle<Component> Entity::GetComponent() {
    assert(IsValid());
    return manager_->GetComponent<Component>(id_);
}

template <typename... Components>
std::tuple<ComponentHandle<Components>...> Entity::GetComponents() {
    assert(IsValid());
    return manager_->GetComponents<Components...>(id_);
}

template <typename Component>
bool Entity::HasComponent() {
    assert(IsValid());
    return manager_->HasComponent<Component>(id_);
}

template <typename Component>
bool ComponentHandle<Component>::IsValid() const {
    return manager_ && manager_->IsValid(id_) && manager_->template HasComponent<Component>(id_);
}

template <typename Component>
Component* ComponentHandle<Component>::operator->() {
    assert(IsValid());
    return manager_->template GetComponentPtr<Component>(id_);
}

template <typename Component>
const Component* ComponentHandle<Component>::operator->() const {
    assert(IsValid());
    return manager_->template GetComponentPtr<Component>(id_);
}

template <typename Component>
Component& ComponentHandle<Component>::operator*() {
    assert(IsValid());
    return *manager_->template GetComponentPtr<Component>(id_);
}

template <typename Component>
const Component& ComponentHandle<Component>::operator*() const {
    assert(IsValid());
    return *manager_->template GetComponentPtr<Component>(id_);
}

template <typename Component>
Component* ComponentHandle<Component>::Get() {
    assert(IsValid());
    return manager_->template GetComponentPtr<Component>(id_);
}

template <typename Component>
const Component* ComponentHandle<Component>::Get() const {
    assert(IsValid());
    return manager_->template GetComponentPtr<Component>(id_);
}

template <typename Component>
void ComponentHandle<Component>::Remove() {
    assert(IsValid());
    manager_->template Remove<Component>(id_);
}

template <typename Component>
Entity ComponentHandle<Component>::GetEntity() {
    assert(IsValid());
    return manager_->GetEntity(id_);
}

};  // namespace ecs

namespace std {
template <>
struct hash<ecs::Entity> {
    std::size_t operator()(const ecs::Entity& entity) const {
        return static_cast<std::size_t>(entity.GetId().GetIndex() ^ entity.GetId().GetGeneration());
    }
};

template <>
struct hash<const ecs::Entity> {
    std::size_t operator()(const ecs::Entity& entity) const {
        return static_cast<std::size_t>(entity.GetId().GetIndex() ^ entity.GetId().GetGeneration());
    }
};
}  // namespace std

#endif
