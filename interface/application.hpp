#ifndef MULTITASK_EMPIRE_INTERFACE_APPLICATION_HPP
#define MULTITASK_EMPIRE_INTERFACE_APPLICATION_HPP

// Multitask Empire
#include <interface/manage/event_manager.hpp>

// Some faggot named this
#include <graphics/aWindow.hpp>

namespace interface {

// This piece of shit is created from
// an existing instance of Application
class Application {
public:
	// Non-default-constructable
	explicit Application(igraphicslib::Window* window);

	// Non-copiable
	Application& operator=(const Application&) = delete;
	Application(const Application&) = delete;

	// Non-movable
	Application& operator=(Application&&) = delete;
	Application(Application&&) = delete;

	~Application();

	static Application* App();

	// One-shot
	void Run();

	// One-shot
	void Exit();

private:
	void Dispatch(const igraphicslib::Event& event);

private:
	static Application* app;

	igraphicslib::Window* window_;

	manage::EventManager* event_manager_;
	bool is_running_;
};

}  // namespace graphics

#endif  // MULTITASK_EMPIRE_INTERFACE_APPLICATION_HPP
