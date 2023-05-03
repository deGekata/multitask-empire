#ifndef H_COLLISION
#define H_COLLISION

#include <unordered_map>

#include <components/collision_components.hpp>
#include <components/movement_components.hpp>

#include <events/player_events.hpp>

#include <ecs/system.hpp>

struct CollideInfo {
  Position pos_;
  Collidable box_;
};

class CollisionSystem : public ecs::System<CollisionSystem>, public ecs::Reciever<CollisionSystem> {
public:  
  void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
  void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

  void Recieve(const PlayerInitiatedEvent& new_player);
private:
  void CollectCandidates(ecs::EntityManager& entities);
  void ProcessCandidates(ecs::EventManager& events);

  static bool IsCollide(const CollideInfo& lhs, const CollideInfo& rhs);
private:
  std::unordered_map<ecs::Entity, CollideInfo> candidates_;
};

#endif
