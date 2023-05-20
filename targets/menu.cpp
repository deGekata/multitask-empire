#include <interface/application.hpp>
#include <interface/objects/button.hpp>

int main() {
	auto* window = new igraphicslib::Window(800, 600, "Test");
	auto* app = new interface::Application(window);

	auto* button = new interface::objects::Button(nullptr, 10, 10, 100, 100, igraphicslib::Text(""));
	interface::manage::EventManager::Current()->Subscribe(button);

	app->Run();

	delete window;
	delete app;

	return 0;
}
