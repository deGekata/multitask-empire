#include <ecs/entity.hpp>
#include <logger/logger.hpp>

namespace ecs {

const Entity::Id Entity::kInvalidId;
BaseComponent::Family BaseComponent::family_counter = 0;

Entity::Id::Id(const uint64_t id) : id_(id) {
}

Entity::Id::Id(const uint32_t index, const uint32_t generation)
    : id_(static_cast<uint64_t>(index) | (static_cast<uint64_t>(generation) << 32ul)) {
}

uint64_t Entity::Id::GetId() const {
    return id_;
}

bool Entity::Id::operator==(const Id& other) const {
    return id_ == other.id_;
}
bool Entity::Id::operator!=(const Id& other) const {
    return id_ != other.id_;
}
bool Entity::Id::operator<(const Id& other) const {
    return id_ < other.id_;
}

uint32_t Entity::Id::GetIndex() const {
    return id_ & 0xffffffff;
}

uint32_t Entity::Id::GetGeneration() const {
    return static_cast<uint32_t>(id_ >> 32);
}

Entity::Entity() : manager_(nullptr), id_(kInvalidId) {
}

Entity::Entity(EntityManager* manager, Id id) : manager_(manager), id_(id) {
}

bool Entity::operator==(const Entity& other) const {
    return other.manager_ == manager_ && other.id_ == id_;
}

bool Entity::operator!=(const Entity& other) const {
    return !(other == *this);
}

bool Entity::operator<(const Entity& other) const {
    return other.id_ < id_;
}

bool Entity::IsValid() const {
    return manager_ && manager_->IsValid(id_);
}

void Entity::Invalidate() {
    manager_ = nullptr;
    id_ = kInvalidId;
}

Entity::Id Entity::GetId() const {
    return id_;
}

EntityManager::EntityManager(EventManager& event_manager)
    : index_counter_(0)
    , event_manager_(event_manager)
    , component_pools_()
    , component_helpers_()
    , entity_component_mask_()
    , entity_generations_()
    , free_list_()
    , tracker_(this) {
}

EntityManager::~EntityManager() {
    Reset();
}

void EntityManager::Reset() {
    for (Entity entity : GetAllEntities()) {
        entity.Destroy();
    }

    for (BasePool* pool : component_pools_) {
        delete pool;
    }

    for (IBaseComponentHelper* helper : component_helpers_) {
        delete helper;
    }

    component_pools_.clear();
    component_helpers_.clear();
    entity_component_mask_.clear();
    entity_generations_.clear();
    free_list_.clear();

    index_counter_ = 0;
}

uint64_t EntityManager::GetCapacity() const {
    return entity_component_mask_.size();
}

bool EntityManager::IsValid(const Entity::Id& id) const {
    return id.GetIndex() < entity_generations_.size() && entity_generations_[id.GetIndex()] == id.GetGeneration();
}

TrackingManager& EntityManager::Tracker() {
    return tracker_;
}

void Entity::Destroy() {
    assert(IsValid());
    manager_->Destroy(id_);
    Invalidate();
}

ComponentMask Entity::GetComponentMask() const {
    return manager_->GetComponentMask(id_);
}

Entity EntityManager::Create() {
    uint32_t index = 0;
    uint32_t generation = 0;

    if (free_list_.empty()) {
        index = index_counter_++;

        //! if number of entities is > MAX_ENTITES
        assert(index_counter_ < kMaxEntities);
        AccomodateEntity(index);

        entity_generations_[index] = 1;
        generation = 1;
    } else {
        index = free_list_.back();
        free_list_.pop_back();

        generation = entity_generations_[index];
    }

    Entity new_entity{this, Entity::Id(index, generation)};
    event_manager_.Emit<EntityCreatedEvent>(new_entity);

    return new_entity;
}

Entity EntityManager::CreateFromCopy(const Entity original) {
    assert(original.IsValid());

    auto clone = Create();
    auto mask = original.GetComponentMask();

    for (uint64_t component_idx = 0; component_idx < component_helpers_.size(); component_idx++) {
        IBaseComponentHelper* helper = component_helpers_[component_idx];

        if (helper && mask.test(component_idx)) {
            helper->CopyComponentTo(original, clone);
        }
    }

    return clone;
}

void EntityManager::Destroy(const Entity::Id entity) {
    AssertId(entity);

    uint32_t index = entity.GetIndex();
    auto mask = entity_component_mask_[index];

    for (uint64_t component_idx = 0; component_idx < component_helpers_.size(); component_idx++) {
        IBaseComponentHelper* helper = component_helpers_[component_idx];

        if (helper && mask.test(component_idx)) {
            helper->RemoveComponent({this, entity});
        }
    }

    event_manager_.Emit<EntityDestroyedEvent>(Entity{this, entity});

    entity_component_mask_[index].reset();
    entity_generations_[index]++;

    free_list_.push_back(index);
}

EntityManager::BaseViewer<true> EntityManager::GetAllEntities() {
    return BaseViewer<true>{this};
}

Entity EntityManager::GetEntity(Entity::Id id) {
    AssertId(id);

    return Entity{this, id};
}

Entity::Id EntityManager::GetId(const uint32_t idx) {
    return Entity::Id{idx, entity_generations_[idx]};
}

void EntityManager::AssertId(const Entity::Id id) const {
    assert((id.GetIndex() < entity_component_mask_.size()) && "Entity::Id ID outside entity vector range");
    assert((entity_generations_[id.GetIndex()] == id.GetGeneration()) &&
           "Attempt to access Entity via a stale Entity::Id");
}

void EntityManager::AccomodateEntity(const uint32_t index) {
    if (entity_component_mask_.size() > index) {
        return;
    }

    entity_component_mask_.resize(index + 1);
    entity_generations_.resize(index + 1);

    for (auto* pool : component_pools_) {
        if (pool) {
            pool->Resize(index + 1);
        }
    }
}

void BaseComponent::operator delete(void*) {
    std::abort();
}

void BaseComponent::operator delete[](void*) {
    std::abort();
}

TrackingManager::TrackingManager(EntityManager* tracking_manager) : tracking_manager_(tracking_manager) {

    tracking_entities_.reset();
    tracking_components_on_adding_.reset();
    tracking_components_on_removing_.reset();
}

void TrackingManager::TrackEntity(uint32_t index) {
    assert(index < tracking_entities_.size() && tracking_manager_);
    tracking_entities_.set(index);
}

void TrackingManager::UnTrackEntity(uint32_t index) {
    assert(index < tracking_entities_.size() && tracking_manager_);
    tracking_entities_.reset(index);
}

bool TrackingManager::IsEntityTracking(uint32_t index) {
    assert(index < tracking_entities_.size() && tracking_manager_);
    return tracking_entities_.test(index);
}

};  // namespace ecs
