#include <cassert>
#include <cstdint>

#include <iostream>
#include <ostream>
#include <string>

#include "../include/quick.hpp"

bool Playing = false;

struct PlayerTag {};

struct PendingMovement {
    PendingMovement(int dx, int dy) : delta_x_(dx), delta_y_(dy) {
    }

    int32_t delta_x_;
    int32_t delta_y_;
};

std::ostream& operator<<(std::ostream& out, PendingMovement& pending) {
    out << "{" << pending.delta_x_ << "; " << pending.delta_y_ << "}";

    return out;
}

class PlayerSystem : public ecs::System<PlayerSystem> {
public:
    void Update(ecs::EntityManager& entities, ecs::TimeDelta) override {
        CheckPlayer(entities);
        ProcessCommand(entities);
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

    void ProcessCommand(ecs::EntityManager& entities) {
        std::string command;
        std::cin >> command;

        if (command == "up") {
            for (auto ent : entities.GetEntitiesWithComponents<PlayerTag>()) {
                if (ent.HasComponent<PendingMovement>()) {
                    ent.GetComponent<PendingMovement>()->delta_y_++;
                } else {
                    ent.Assign<PendingMovement>(0, 1);
                }

                std::cout << "Processed \"up\" command: entity " << ent.GetId().GetIndex()
                          << " now has pending movement " << *ent.GetComponent<PendingMovement>() << std::endl;
            }
        } else if (command == "down") {
            for (auto ent : entities.GetEntitiesWithComponents<PlayerTag>()) {
                if (ent.HasComponent<PendingMovement>()) {
                    ent.GetComponent<PendingMovement>()->delta_y_--;
                } else {
                    ent.Assign<PendingMovement>(0, -1);
                }

                std::cout << "Processed \"down\" command: entity " << ent.GetId().GetIndex()
                          << " now has pending movement " << *ent.GetComponent<PendingMovement>() << std::endl;
            }
        } else if (command == "left") {
            for (auto ent : entities.GetEntitiesWithComponents<PlayerTag>()) {
                if (ent.HasComponent<PendingMovement>()) {
                    ent.GetComponent<PendingMovement>()->delta_x_--;
                } else {
                    ent.Assign<PendingMovement>(-1, 0);
                }

                std::cout << "Processed \"left\" command: entity " << ent.GetId().GetIndex()
                          << " now has pending movement " << *ent.GetComponent<PendingMovement>() << std::endl;
            }
        } else if (command == "right") {
            for (auto ent : entities.GetEntitiesWithComponents<PlayerTag>()) {
                if (ent.HasComponent<PendingMovement>()) {
                    ent.GetComponent<PendingMovement>()->delta_x_++;
                } else {
                    ent.Assign<PendingMovement>(1, 0);
                }

                std::cout << "Processed \"right\" command: entity " << ent.GetId().GetIndex()
                          << " now has pending movement " << *ent.GetComponent<PendingMovement>() << std::endl;
            }
        } else if (command == "exit") {
            Playing = false;
        } else {
            std::cout << "Unknown command" << std::endl;
        }
    }
};

class Application : public ecs::ECS {
public:
    Application() {
        systems_.Add<PlayerSystem>();

        systems_.Configure();
        Playing = true;
    }

    void Update() {
        systems_.UpdateAll(0);
    }
};

int main() {
    Application game;

    while (Playing) {
        game.Update();
    }
}
