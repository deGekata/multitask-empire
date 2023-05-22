#include <interface/application.hpp>

#include <interface/objects/image.hpp>
#include <interface/objects/label.hpp>
#include <interface/objects/button.hpp>

#include <audio/manage/audio_manager.hpp>

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

	exit->SetOnClick([]() {
		interface::Application::App()->Exit();
	});
	exit->SetOnHoverIn([exit]() {
		exit->SetColor(igraphicslib::colors::kPink);
	});
	exit->SetOnHoverOut([exit]() {
		exit->SetColor(igraphicslib::colors::kWhite);
	});

	auto* next = new interface::objects::Button({320, 100, 210, 100});
	next->SetLabel("Next");

	next->SetOnClick([&audio_manager]() {
		audio_manager.MusicNextSong();
	});
	next->SetOnHoverIn([next]() {
		next->SetColor(igraphicslib::colors::kPink);
	});
	next->SetOnHoverOut([next]() {
		next->SetColor(igraphicslib::colors::kWhite);
	});

	auto* prev = new interface::objects::Button({580, 100, 180, 100});
	prev->SetLabel("Prev");

	prev->SetOnClick([&audio_manager]() {
		audio_manager.MusicPrevSong();
	});
	prev->SetOnHoverIn([prev]() {
		prev->SetColor(igraphicslib::colors::kPink);
	});
	prev->SetOnHoverOut([prev]() {
		prev->SetColor(igraphicslib::colors::kWhite);
	});

	auto* play = new interface::objects::Button({810, 100, 180, 100});
	play->SetLabel("Play");

	play->SetOnClick([]() {
		interface::Application::App()->Exit();
	});
	play->SetOnHoverIn([play]() {
		play->SetColor(igraphicslib::colors::kPink);
	});
	play->SetOnHoverOut([play]() {
		play->SetColor(igraphicslib::colors::kWhite);
	});

	return app.Run();
}
