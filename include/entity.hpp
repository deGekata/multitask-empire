#pragma once

#include <bitset>
#include <tuple>
#include <cstdint>
#include <pool.hpp>
#include <vector>

namespace ecs {
class Entity {
public:
    class Id {
    public:
        explicit Id(const uint64_t id = 0);
        Id(const uint32_t index, const uint32_t generation);

        uint64_t GetId() const;

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

    bool IsValid() const;
    void Invalidate();

    Id GetId() const;

public:
    template <typename Component, typename... Args>
    ComponentHandle<Component> Assign(Args&&... args) {
        assert(IsValid());

        return manager_->assign<Component>(id_, std::forward<Args>(args)...);
    }

    template <typename Component>
    ComponentHandle<Component> AssignFromCopy(const Component& component) {
        assert(IsValid());

        return manager_->Assign<Component>(id_, std::forward<const Component&>(component));
    }

    template <typename Component, typename... Args>
    ComponentHandle<Component> Replace(Args&&... args) {
        assert(IsValid());

        auto handle = Component<Component>();
        if (handle) {
            *(handle.get())
        }
    }

    template <typename Component>
    void Remove() {
        assert(IsValid() && HasComponent<Component>());
        manager_->Remove<Component>(id_);
    }

    template <typename Component>
    ComponentHandle<Component> GetComponent() {
        assert(IsValid());
        return manager_->GetComponent<Component>(id_);
    }

    template <typename... Components>
    std::tuple<ComponentHandle<Components>...> GetComponents() {
        assert(IsValid());
        return manager_->GetComponents<Components...>(id_);
    }

    template <typename Component>
    bool HasComponent() {
        assert(IsValid());
        return manager_->HasComponent<Component>(id_);
    }

    /**
     * Destroy and invalidate this Entity.
     */
    void Destroy();

    EntityManager::ComponentMask GetComponentMask() const;

private:
    EntityManager* manager_;
    Id id_;
};

constexpr uint64_t MAX_COMPONENTS = 128;

struct EntityManager {
public:
    // TODO std::vector<bool>
    using ComponentMask = std::bitset<MAX_COMPONENTS>;

    void Reset();

    bool IsValid(const Entity::Id& id) const;

    Entity Create();
    Entity CreateFromCopy(const Entity original);

    void Destroy(const Entity::Id entity);

    // Retrieve the component family for a type.
    template <typename Component>
    static BaseComponent::Family Family() {
        return Component<typename std::remove_const<Component>::type>::GetFamily();
    }

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

        entity_component_mask_[id.GetIndex()].reset(family);
        pool->Destroy(id.GetIndex());
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

private:
    friend class Entity;

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
    ComponentMask GetComponentMask(const ComponentHandle<C>& c) {
        return GetComponentMask<C>();
    }

    template <typename C1, typename... Components>
    ComponentMask GetComponentMask(const ComponentHandle<C1>& c1, const ComponentHandle<Components>&... args) {
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
            auto* pool = new Pool<Conmponent>();
            pool->Resize(index_counter_);

            component_pools_[family] = pool;
        }

        if (component_helpers_.size() <= family) {
            component_helpers_.reserve(family + 1, nullptr);
        }
        if (component_helpers_[family] == nullptr) {
            ComponentHelper<Component>* helper = new ComponentHelper<C>();

            component_helpers_[family] = helper;
        }

        return static_cast<Pool<Component>*>(component_pools_[family]);
    }

    uint32_t index_counter_ = 0;

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

/**
 * Base component class, only used for insertion into collections.
 *
 * Family is used for registration.
 */
struct BaseComponent {
public:
    using Family = uint64_t;

    void operator delete(void* p);
    void operator delete[](void* p);

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
    friend class EntityManager;
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

    bool IsValid() const {
        return manager_ && manager_->IsValid(id_) && manager_->template HasComponent<Component>(id_);
    }

    Component* operator->() {
        assert(IsValid());
        return manager_->template GetComponentPtr<Component>(id_);
    }

    const Component* operator->() const {
        assert(IsValid());
        return manager_->template GetComponentPtr<Component>(id_);
    }

    Component& operator*() {
        assert(IsValid());
        return *manager_->template GetComponentPtr<Component>(id_);
    }

    const Component& operator*() const {
        assert(IsValid());
        return *manager_->template GetComponentPtr<Component>(id_);
    }

    Component* Get() {
        assert(IsValid());
        return manager_->template GetComponentPtr<Component>(id_);
    }

    const Component* Get() const {
        assert(IsValid());
        return manager_->template GetComponentPtr<Component>(id_);
    }

    void Remove() {
        assert(IsValid());
        manager_->template Remove<Component>(id_);
    }

    Entity GetEntity() {
        assert(IsValid());
        return manager_->GetEntity(id_);
    }

private:
    friend class EntityManager;

    ComponentHandle(EntityManager* manager, Entity::Id id) : manager_(manager), id_(id) {
    }

    EntityManager* manager_;
    Entity::Id id_;
};

struct BaseComponentHelper {
    virtual ~BaseComponentHelper() {
    }
    virtual void RemoveComponent(Entity enity) = 0;
    virtual void CopyComponentTo(Entity from, Entity to);
};

template <typename Component>
struct ComponentHelper : public BaseComponentHelper {
    void RemoveComponent(Entity entity) override {
        entity.remove<Component>();
    }

    void CopyComponentTo(Entity from, Entity to) {
        to.assign_from_copy<Component>(*(from.component<C>().get()));
    }
};
};  // namespace ecs
