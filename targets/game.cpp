#include <cassert>
#include <cstdint>

#include <iostream>
#include <ostream>
#include <string>

#include <ecs/quick.hpp>

#include <physics/movement.hpp>
#include <physics/mv_commands.hpp>
#include <physics/friction.hpp>
#include <physics/gravitation.hpp>

#include <player/dispatcher.hpp>
#include <player/player.hpp>

#include <logger/logger.hpp>
#include <logger/logger_ecs.hpp>

class Application : public ecs::ECS {
public:
    Application() : running_(true) {
        systems_.Add<EcsFullLogger>();
        systems_.Add<PlayerSystem>();

        systems_.Add<DispatcherSystem>(running_);
        systems_.Add<MovementCommandsSystem>();
        
        systems_.Add<GravitationSystem>();
        systems_.Add<FrictionSystem>();
        systems_.Add<MovementSystem>();

        systems_.Configure();

        log<INFO>("Application sucessfully created\n");
    }

    void Update(ecs::TimeDelta dt) {
        systems_.UpdateAll(dt);
    }

    bool GetState() {
        return running_;
    }

private:
    bool running_;
};

int main() {
    Application game;

    // auto prev_timer = std::chrono::steady_clock::now();
    while (game.GetState()) {
        // auto new_timer = std::chrono::steady_clock::now();
        // auto dt = std::chrono::duration_cast<std::chrono::seconds>(new_timer - prev_timer);

        // std::cout << "Delta is " << dt.count() << std::endl;
        game.Update(1);

        // prev_timer = new_timer;
    }
}
