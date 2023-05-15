#include <collision/stopper.hpp>

#include <components/collision_components.hpp>
#include <components/movement_components.hpp>

void CollisionStopperSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<CollisionEvent>(*this);
}

void CollisionStopperSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void CollisionStopperSystem::Receive(const CollisionEvent& collided_pair) {
    ecs::Entity first_entity = collided_pair.first_collided_;
    ecs::Entity second_entity = collided_pair.second_collided_;

    if (!first_entity.HasComponent<CollidableTag>() || !second_entity.HasComponent<CollidableTag>()) {
        return;
    }

    auto pos_first = first_entity.GetComponent<Position>();
    auto pos_second = second_entity.GetComponent<Position>();

    if (pos_second->x_ < pos_first->x_) {
        std::swap(pos_first, pos_second);
    }

    auto box_first = first_entity.GetComponent<HitBox>();
    auto box_second = second_entity.GetComponent<HitBox>();

    int64_t delta_x = pos_second->x_ - pos_first->x_;
    pos_second->x_ += ((box_first->width_ + box_second->width_) / 2) - delta_x;
    pos_first->x_ -= ((box_first->width_ + box_second->width_) / 2) - delta_x;
}
