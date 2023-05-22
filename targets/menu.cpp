#include <interface/application.hpp>

#include <interface/objects/image.hpp>
#include <interface/objects/label.hpp>
#include <interface/objects/button.hpp>

#include <audio/manage/audio_manager.hpp>

#include <cassert>
#include <cstdint>

#include <iostream>
#include <ostream>
#include <string>
#include <thread>
#include <chrono>

#include <ecs/quick.hpp>

#include <bot/bot.hpp>

#include <battle/attack.hpp>
#include <battle/block.hpp>
#include <battle/curses.hpp>
#include <battle/fire.hpp>
#include <battle/potion.hpp>
#include <battle/slime.hpp>
#include <battle/special.hpp>
#include <battle/skill_change.hpp>
#include <battle/health.hpp>

#include <bars/special_bar.hpp>
#include <bars/shield_bar.hpp>
#include <bars/skill_bar.hpp>
#include <bars/health_bar.hpp>

#include <collision/collision.hpp>
#include <collision/stopper.hpp>

#include <gameplay/controller.hpp>

#include <physics/movement.hpp>
#include <physics/mv_commands.hpp>
#include <physics/friction.hpp>
#include <physics/limiter.hpp>
#include <physics/gravitation.hpp>

#include <player/player.hpp>
#include <player/input.hpp>
#include <player/text_input.hpp>

#include <renderer/renderer.hpp>

#include <logger/logger.hpp>
#include <logger/logger_ecs.hpp>

#include <objects_configuration/spritesheet.hpp>
#include <objects_configuration/battleable_configuration.hpp>
#include <objects_configuration/battleable_state_switcher.hpp>

#include <parsing/character_config_parser.hpp>

#include <utility/attach.hpp>

#include <audio/audio_system.hpp>
class Application : public ecs::ECS {
public:
    Application() {
        systems_.Add<EcsFullLogger>();

        systems_.Add<AudioSystem>();
        systems_.Add<KeyboardInputSystem>();
        systems_.Add<RendererSystem>();
        systems_.Add<SpriteSheetSystem>();
        systems_.Add<BattleAbleObjectsConfigSystem>();
        systems_.Add<BattleAbleStateSwitchSystem>();
        systems_.Add<TextInputSystem>();

        systems_.Add<MovementCommandsSystem>();

        systems_.Add<GravitationSystem>();
        systems_.Add<FrictionSystem>();
        systems_.Add<MovementSystem>();
				systems_.Add<LimiterSystem>();

        systems_.Add<CollisionSystem>();
        systems_.Add<CollisionStopperSystem>();

        systems_.Add<HealthSystem>();
        systems_.Add<AttackSystem>();
        systems_.Add<BlockSystem>();
        systems_.Add<CursesSystem>();
        systems_.Add<FireSystem>();
        systems_.Add<PotionSystem>();
        systems_.Add<SlimeSystem>();
        systems_.Add<SpecialSystem>();
        systems_.Add<SkillChangeSystem>();

        systems_.Add<HealthBarSystem>();
        systems_.Add<SpecialBarSystem>();
        systems_.Add<ShieldBarSystem>();
        systems_.Add<SkillBarSystem>();

        systems_.Add<AttachSystem>();

        systems_.Add<PlayerSystem>();
        systems_.Add<BotSystem>();

        systems_.Add<ControllerSystem>();

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
        auto prev_timer = std::chrono::steady_clock::now();

        while (GetState()) {
            auto new_timer = std::chrono::steady_clock::now();
            auto dt = std::chrono::duration_cast<std::chrono::nanoseconds>(new_timer - prev_timer);

            Update(static_cast<double>(dt.count()) / 1000.0);

            prev_timer = new_timer;
        }
    }

    void Init() {

        auto* system = reinterpret_cast<KeyboardInputSystem*>(systems_.GetSystem<KeyboardInputSystem>());
        std::thread input_thread(&KeyboardInputSystem::Pool, system);
        std::thread main_thread(&Application::Pool, this);

        input_thread.join();
        main_thread.join();
    }
    // bool running_;
};

int main() {
    audio::manage::AudioManager audio_manager("sounds/", "music/");
    audio_manager.MusicPlay();

    igraphicslib::Window window(1920, 1080, "Multitask-Empire");
    interface::Application app(&window);

    auto* menu_background = new interface::objects::Image({0, 0, 1920, 1080}, "assets/images/menu_bg.jpg");
    auto* logo = new interface::objects::Image({0, 200, 1000, 1000}, "assets/images/logo.png");
    auto* headphones = new interface::objects::Image({1200, 400, 600, 600}, "assets/images/headphohes.png");

    auto* animation1 = new interface::objects::Image({1050, 250, 800, 800}, "assets/images/animation1.png");
    animation1->SetOnTick([animation1]() {
        static int32_t tick_count = 0;
        tick_count++;
        if (tick_count % 100 >= 50) {
            animation1->Draw();
        }
    });

    auto* animation2 = new interface::objects::Image({1050, 250, 800, 800}, "assets/images/animation2.png");
    animation2->SetOnTick([animation2]() {
        static int32_t tick_count = 0;
        tick_count++;
        if (tick_count % 100 < 50) {
            animation2->Draw();
        }
    });

    auto* exit = new interface::objects::Button({100, 100, 170, 100});
    exit->SetLabel("Exit");

    exit->SetOnClick([]() { interface::Application::App()->Exit(); });
    exit->SetOnHoverIn([exit]() { exit->SetColor(igraphicslib::colors::kPink); });
    exit->SetOnHoverOut([exit]() { exit->SetColor(igraphicslib::colors::kWhite); });

    auto* next = new interface::objects::Button({320, 100, 210, 100});
    next->SetLabel("Next");

    next->SetOnClick([&audio_manager]() { audio_manager.MusicNextSong(); });
    next->SetOnHoverIn([next]() { next->SetColor(igraphicslib::colors::kPink); });
    next->SetOnHoverOut([next]() { next->SetColor(igraphicslib::colors::kWhite); });

    auto* prev = new interface::objects::Button({580, 100, 180, 100});
    prev->SetLabel("Prev");

    prev->SetOnClick([&audio_manager]() { audio_manager.MusicPrevSong(); });
    prev->SetOnHoverIn([prev]() { prev->SetColor(igraphicslib::colors::kPink); });
    prev->SetOnHoverOut([prev]() { prev->SetColor(igraphicslib::colors::kWhite); });

    auto* play = new interface::objects::Button({810, 100, 180, 100});
    play->SetLabel("Play");

    play->SetOnClick([&window, &audio_manager]() {
        interface::Application::App()->Exit();
        window.Hide();
        audio_manager.MusicNextSong();
        Application game;
        game.Init();
    });

    play->SetOnHoverIn([play]() { play->SetColor(igraphicslib::colors::kPink); });
    play->SetOnHoverOut([play]() { play->SetColor(igraphicslib::colors::kWhite); });

    return app.Run();
}
