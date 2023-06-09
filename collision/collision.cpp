#include <collision/collision.hpp>

#include <events/collision_events.hpp>

//!
constexpr static double kDefaultWidth = 130.0;
constexpr static double kDefaultHeight = 100.0;

void CollisionSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerInitiatedEvent>(*this);
}

void CollisionSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    candidates_.clear();
    CollectCandidates(entities);
    ProcessCandidates(events);
}

void CollisionSystem::CollectCandidates(ecs::EntityManager& entities) {
    entities.Each<Position, HitBox>([this](const ecs::Entity& entity, const Position& pos, const HitBox& box) {
        candidates_[entity] = {pos, box};
    });
}

void CollisionSystem::ProcessCandidates(ecs::EventManager& events) {
    for (auto candidate_one = candidates_.begin(); candidate_one != candidates_.end(); candidate_one++) {
        for (auto candidate_two = candidate_one; candidate_two != candidates_.end(); candidate_two++) {
            if (candidate_one->first == candidate_two->first) {
                continue;
            }

            if (IsCollide(candidate_one->second, candidate_two->second)) {
                events.Emit<CollisionEvent>(candidate_one->first, candidate_two->first);
            }
        }
    }
}

bool CollisionSystem::IsCollide(const CollideInfo& lhs, const CollideInfo& rhs) {
    return (std::abs(lhs.pos_.x_ - rhs.pos_.x_) <= ((lhs.box_.width_ + rhs.box_.width_) / 2.0)) &&
           (std::abs(lhs.pos_.y_ - rhs.pos_.y_) <= ((lhs.box_.height_ + rhs.box_.height_) / 2.0));
}

void CollisionSystem::Receive(const PlayerInitiatedEvent& new_player) {
    ecs::Entity entity = new_player.entity_;

    entity.Assign<HitBox>(kDefaultWidth, kDefaultHeight);
    entity.Assign<CollidableTag>();
}
