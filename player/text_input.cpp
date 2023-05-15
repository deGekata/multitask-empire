#include <player/text_input.hpp>
#include <iostream>
#include <sstream>

void TextInputSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
    events_ = &events;
}

void TextInputSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void TextInputSystem::Receive(const PlayerCommandEvent& event) {
    if (event.cmd_ == PlayerCommand::TEXT_INSERT_REQUEST) {
        std::string cmd;
        std::cout << "> ";

        std::getline(std::cin, cmd);
        // todo: refactor
        std::stringstream ss(cmd);

        events_->Emit<PlayerTextRequestEvent>(std::move(ss));
    }
}
