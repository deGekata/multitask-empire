#include <interface/application.hpp>

#include <utility/time.hpp>

namespace interface {

Application* Application::app = nullptr;

Application::Application(igraphicslib::Window* window)
	: window_(window)
	, event_manager_(new manage::EventManager())
	, is_running_(false) {

	assert(app == nullptr);

	app = this;
}

Application::~Application() {
	delete event_manager_;
}

Application* Application::App() {
	return app;
}

igraphicslib::Window* Application::Window() {
	return app->window_;
}

void Application::Run() {
	assert(window_ != nullptr);
	assert(event_manager_ != nullptr);

	is_running_ = true;

	// ~60 fps
	const utility::Time frame_delay = utility::milliseconds(16);

	while (is_running_) {
		utility::Clock clock; 

		window_->Clear();

		// Proccess all the events in the queue
		igraphicslib::Event event;
		while (window_->PollEvent(event)) {
			Dispatch(event);
		}

		event_manager_->ProcessTimerTick(clock.getElapsedTime());
		window_->Update();

		utility::Delay(frame_delay - clock.getElapsedTime());
	}
}

void Application::Exit() {
	is_running_ = false;
}

void Application::Dispatch(const igraphicslib::Event& event) {
	switch (event.type) {
        case igraphicslib::EventType::ERROR: {
        	Exit();
        	break;
        }

        case igraphicslib::EventType::MouseButtonPressed: {
        	event_manager_->ProcessButtonPress(event.mbed);
        	break;
        }

        case igraphicslib::EventType::MouseButtonReleased: {
        	event_manager_->ProcessButtonRelease(event.mbed);
        	break;
        }

        case igraphicslib::EventType::MouseMoved: {
        	event_manager_->ProcessMouseMovement(event.mmed);
        	break;
        }

        case igraphicslib::EventType::MouseWheeled: {
        	event_manager_->ProcessWheelScroll(event.mwed);
        	break;
        }

        case igraphicslib::EventType::KeyPressed: {
        	event_manager_->ProcessKeyPress(event.ked);
        	break;
        }

        case igraphicslib::EventType::KeyReleased: {
        	event_manager_->ProcessKeyRelease(event.ked);
        	break;
        }

        default: {
        	fmt::println("Unsupported event type");
        }
	}
}

}  // namespace intrface
