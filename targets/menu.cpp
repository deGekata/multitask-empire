#include <interface/application.hpp>

#include <interface/objects/image.hpp>
#include <interface/objects/label.hpp>
#include <interface/objects/button.hpp>

int main() {
	auto* window = new igraphicslib::Window(1920, 1080, "Multitask-Empire");
	auto* app = new interface::Application(window);

	auto* image = new interface::objects::Image({0, 0, 1920, 1080}, "assets/images/menu_bg.jpg");

	auto* button = new interface::objects::Button({100, 100, 500, 500});
	button->SetLabel("Exit");
	button->SetCallback([]() {
		interface::Application::App()->Exit();
	});

	int ret = app->Run();

	delete window;
	delete app;

	return ret;
}
