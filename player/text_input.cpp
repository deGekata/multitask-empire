#include <player/text_input.hpp>
#include <iostream>
#include <sstream>

void TextInputSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
    events_ = &events;
}

void TextInputSystem::Update(ecs::EntityManager&, ecs::EventManager&, ecs::TimeDelta) {
}

void TextInputSystem::Receive(const PlayerCommandEvent& event) {

    ecs::Entity cmd_ent = event.cmd_;

    auto cmd_type = cmd_ent.GetComponent<PlayerCommand>().Get();
    if(cmd_type->type_ != PlayerCommandType::Action) return;

    if (cmd_type->type_ == PlayerCommandType::TextInsertRequest) {

        // todo: to queue
        std::string cmd;
        std::cout << "> ";

        std::getline(std::cin, cmd);
        // todo: refactor
        std::stringstream ss(cmd);

        events_->Emit<PlayerTextRequestEvent>(std::move(ss));
    }
}
