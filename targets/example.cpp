#include <cassert>
#include <cstdint>

#include <iostream>

#include "../include/quick.hpp"

struct PlayerTag {};

struct PendingMovement {
  int32_t delta_x_;
  int32_t delta_y_;
};

class PlayerSystem : public ecs::System<PlayerSystem> {
public:
    void Update(ecs::EntityManager& entities, ecs::TimeDelta) override {
      CheckPlayer(entities);
    }

private:
    void CheckPlayer(ecs::EntityManager& entities) {
        uint32_t counter = 0;
        entities.Each<PlayerTag>([&](ecs::Entity, PlayerTag&) { counter++; });

        assert(counter <= 1);
        if (counter == 0) {
          ecs::Entity entity = entities.Create();
          std::cout << "Created entity Player with id = " << entity.GetId().GetId() << std::endl;

          entity.Assign<PlayerTag>();
        }
    }
};

class Application : public ecs::ECS {
public:
    Application() {
        systems_.Add<PlayerSystem>();

        systems_.Configure();
    }

    void Update() {
      systems_.UpdateAll(0);
    }
};

int main() {
  Application game;

  while (true) {
    game.Update();
  }
}
