#include <interface/application.hpp>

int main() {
	auto* window = new igraphicslib::Window(800, 600, "Test");
	auto* app = new interface::Application(window);

	app->Run();

	delete window;
	delete app;

	return 0;
}
