#include <player/input.hpp>
#include <ecs/entity.hpp>
#include <components/player_components.hpp>

#define ADD_CMD_MATCH(cmd, key) \
    key_to_cmd_matcher_[static_cast<uint>(igraphicslib::KeyboardKey::key)] = PlayerCommand::cmd;

void KeyboardInputSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<WindowInitiatedEvent, KeyboardInputSystem>(*this);
    events.Subscribe<WindowClosedEvent, KeyboardInputSystem>(*this);

    ADD_CMD_MATCH(WALK_LEFT, A)
    ADD_CMD_MATCH(WALK_RIGHT, D)
    ADD_CMD_MATCH(ATTACK_ONE, J)
    ADD_CMD_MATCH(ATTACK_TWO, K)
    ADD_CMD_MATCH(DEATH, S)
    ADD_CMD_MATCH(JUMP, W)
    ADD_CMD_MATCH(TEXT_INSERT_REQUEST, T)

    cur_key_ = igraphicslib::KeyboardKey::Unknown;
}

void KeyboardInputSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    entities.Each<PlayerTag>([this, &events](ecs::Entity player, PlayerTag&) {
        for (auto event = events_queue_.front(); !events_queue_.empty(); events_queue_.pop_front()) {
            if (event.type == igraphicslib::EventType::KeyPressed) {
                if (event.ked.key == cur_key_) {
                    continue;
                }

                cur_key_ = event.ked.key;

                PlayerCommand matched_cmd = key_to_cmd_matcher_[static_cast<uint>(event.ked.key)];
                if (matched_cmd != PlayerCommand::INVALID) {
                    events.Emit<PlayerCommandEvent>(matched_cmd, player);
                }
            } else if (event.type == igraphicslib::EventType::KeyReleased) {
                cur_key_ = igraphicslib::KeyboardKey::Unknown;
                PlayerCommand matched_cmd = key_to_cmd_matcher_[static_cast<uint>(event.ked.key)];

                if (matched_cmd != PlayerCommand::INVALID) {
                    events.Emit<PlayerCommandEvent>(PlayerCommand::IDLE, player);
                }
            }
        }
    });
}

void KeyboardInputSystem::Recieve(const WindowInitiatedEvent& event) {
    if (!pooling_lock_.try_lock()) {
        std::cout << "WARNING, unable to link window to keyboard input system, because pooling is online\n";
        return;
    }

    root_window_ = event.p_window_;
    pooling_lock_.unlock();
}

void KeyboardInputSystem::Recieve(const WindowClosedEvent& event) {
    if (!pooling_lock_.try_lock()) {
        std::cout << "WARNING, unable to link window to keyboard input system, because pooling is online\n";
        return;
    }

    if (root_window_ == event.p_window_) {
        root_window_ = nullptr;
    }

    pooling_lock_.unlock();
}

void KeyboardInputSystem::Pool() {
    std::lock_guard<std::mutex> guard(pooling_lock_);
    assert(root_window_);

    while (!is_request_to_stop_pooling_.load()) {
        igraphicslib::Event event;
        if (root_window_->PollEvent(event)) {
            if (event.type == igraphicslib::EventType::KeyPressed ||
                event.type == igraphicslib::EventType::KeyReleased) {
                events_queue_.push_back(event);
            }
        }
    }
}

void KeyboardInputSystem::StopPool() {
    is_request_to_stop_pooling_.store(true);

    // if we able to lock pooling_lock_, it means that pooling has stopped
    std::lock_guard<std::mutex> guard(pooling_lock_);
}
