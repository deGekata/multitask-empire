// #include <player/dispatcher.hpp>

// #include <iostream>

// #include <components/movement_components.hpp>
// #include <components/player_components.hpp>

// #include <events/movement_events.hpp>

// DispatcherSystem::DispatcherSystem(bool& flag) : running_(flag) {
// }

// void DispatcherSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
//     events.Subscribe<PlayerCommandEvent>(*this);
// }

// void DispatcherSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
//     entities.Each<PlayerTag>([this, &entities, &events](ecs::Entity entity, PlayerTag&) {
//         for (auto cmd = events_queue_.front(); !events_queue_.empty(); events_queue_.pop_front()) {
//             if (cmd.command_ == "JUMP") {
//                 PendingMovementEvent event{entity, MovementCommand::Jump};
//                 events.Emit<PendingMovementEvent>(event);
//             } else if (cmd.command_ == "LEFT") {
//                 PendingMovementEvent event{entity, MovementCommand::Left};
//                 events.Emit<PendingMovementEvent>(event);
//             } else if (cmd.command_ == "RIGHT") {
//                 PendingMovementEvent event{entity, MovementCommand::Right};
//                 events.Emit<PendingMovementEvent>(event);
//             } else if (cmd.command_ == "SKIP") {
//             } else if (cmd.command_ == "SPRITE_SHEET"){
//                 std::string xml_path;
//                 std::cout << "enter the path: ";
//                 std::cin >> xml_path;
//                 events.Emit<SpriteSheetLoadRequest>(xml_path);
//             } else if (cmd.command_ == "SKIN"){
//                 std::string skin_name;
//                 std::cout << "enter the name: ";
//                 std::cin >> skin_name;
//                 events.Emit<SkinChangeRequest>(skin_name, *entities.GetEntitiesWithComponents<PlayerTag>().begin());
//             } else if (cmd.command_ == "IDLE"){
//                 events.Emit<PlayerStateChanged>(PLAYER_STATE::DEFAULT);
//             } if (cmd.command_ == "EXIT") {
//                 running_ = false;
//             }
//         }
//     });
// }

// void DispatcherSystem::Receive(const PlayerCommandEvent& cmd) {
//     events_queue_.push_back(cmd);
// }
