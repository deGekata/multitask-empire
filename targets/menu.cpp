#include <interface/application.hpp>

#include <interface/objects/image.hpp>
#include <interface/objects/label.hpp>

int main() {
	auto* window = new igraphicslib::Window(1920, 1080, "Test");
	auto* app = new interface::Application(window);

	auto* image = new interface::objects::Image({0, 0, 1920, 1080}, "assets/images/menu_bg.png");
	auto* label = new interface::objects::Label({500, 500, 100, 100}, "HUY TEBE A NE SKRIN");

	app->Run();

	delete image;
	delete label;

	delete window;
	delete app;

	return 0;
}
