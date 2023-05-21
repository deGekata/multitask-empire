#include <interface/application.hpp>

#include <interface/objects/image.hpp>
#include <interface/objects/label.hpp>
#include <interface/objects/button.hpp>

#include <sound/music.hpp>

int main() {
	auto* window = new igraphicslib::Window(1920, 1080, "Multitask-Empire");
	auto* app = new interface::Application(window);

	auto* menu_background = new interface::objects::Image({0, 0, 1920, 1080}, "assets/images/menu_bg.jpg");

	auto* button = new interface::objects::Button({100, 100, 320, 200});
	button->SetImage("assets/images/menu_bg.png");
	button->SetLabel("Exit");

	button->SetOnClick([]() {
		interface::Application::App()->Exit();
	});

	button->SetOnHoverIn([button]() {
		button->SetCharacterSize(220);
		button->Resize(340, 200);
	});

	button->SetOnHoverOut([button]() {
		button->SetCharacterSize(200);
		button->Resize(320, 200);
	});

	int ret = app->Run();

	delete window;
	delete app;

	return ret;
}
