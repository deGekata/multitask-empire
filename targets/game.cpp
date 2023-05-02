#include <cassert>
#include <cstdint>

#include <iostream>
#include <ostream>
#include <string>
#include <thread>
#include <chrono>

#include <ecs/quick.hpp>

#include <physics/movement.hpp>
#include <physics/mv_commands.hpp>
#include <physics/friction.hpp>
#include <physics/gravitation.hpp>

#include <player/player.hpp>
#include <player/input.hpp>

#include <renderer/renderer.hpp>

#include <logger/logger.hpp>
#include <logger/logger_ecs.hpp>

#include <spritesheet/spritesheet.hpp>

// #include <graphics/aWindow.hpp>
// #include <graphics/aSprite.hpp>
// #include <graphics/aText.hpp>
// #include <graphics/aTexture.hpp>
// #include <graphics/color.hpp>

class Application : public ecs::ECS {
public:
    Application() : running_(true) {
        systems_.Add<EcsFullLogger>();

        systems_.Add<KeyboardInputSystem>();
        systems_.Add<RendererSystem>();
        systems_.Add<SpriteSheetSystem>();
        
        systems_.Add<PlayerSystem>();

        // systems_.Add<DispatcherSystem>(running_);
        systems_.Add<MovementCommandsSystem>();

        systems_.Add<GravitationSystem>();
        systems_.Add<FrictionSystem>();
        systems_.Add<MovementSystem>();

        systems_.Configure();

        logger::Print(kInfo, "Application sucessfully created\n");
    }

    void Update(ecs::TimeDelta dt) {
        systems_.UpdateAll(dt);
    }

    bool GetState() {
        return true;
    }

    void Pool() {
        while (GetState()) {
                // auto new_timer = std::chrono::steady_clock::now();
                // auto dt = std::chrono::duration_cast<std::chrono::seconds>(new_timer - prev_timer);

                // std::cout << "Delta is " << dt.count() << std::endl;
                
                Update(1);

                // prev_timer = new_timer;
            }
    }

    void Init() {

        KeyboardInputSystem* system = reinterpret_cast<KeyboardInputSystem*>(systems_.GetSystem<KeyboardInputSystem>());
        std::thread input_thread(&KeyboardInputSystem::Pool, system);
        std::thread main_thread(&Application::Pool, this);

        input_thread.join();
        main_thread.join();
    }
private:
    bool running_;
};

// [[noreturn]] void TestWindow() {
//     const char* player_texture_path = "assets/graphics_test/player_sprite.png";
//     igraphicslib::Window window(1000, 800, "Simple");
//     window.Show();
//     igraphicslib::Texture player_texture(player_texture_path);
//     igraphicslib::Sprite player_sprite(player_texture);
//     igraphicslib::Rect first_pos(0u, 0u, 40u, 100u);
//     igraphicslib::Rect second_pos(0u, 120u, 40u, 100u);
//     bool changed = false     while (true) {
//         window.Clear();
//         window.DrawPoint({100, 100}, igraphicslib::colors::kGreen);
//         window.DrawLine({14, 40}, {500, 500}, igraphicslib::colors::kRed);
//         // window.Draw
//         if (changed) {
//             auto tmp = player_sprite.Crop(first_pos);
//             tmp.SetScale(3, 3);
//             window.DrawSprite({100, 100}, tmp);
//             changed = !changed;
//         } else {
//             auto tmp = player_sprite.Crop(second_pos);
//             tmp.SetScale(3, 3);
//             window.DrawSprite({100, 100}, tmp);
//             changed = !changed;
//         }

//         window.DrawHolRect({600, 1}, {950, 101}, igraphicslib::colors::kRed);
//         window.DrawRect({700, 2, 250, 97}, igraphicslib::colors::kMagenta);
//         window.Update();
//         std::this_thread::sleep_for(std::chrono::milliseconds(500));
        
//     }


// }

int main() {
    Application game;
    game.Init();
    // auto prev_timer = std::chrono::steady_clock::now();
}
