#include <player/player.hpp>

#include <events/player_events.hpp>
#include <events/renderer_events.hpp>

#include <components/movement_components.hpp>
#include <components/player_components.hpp>
#include <components/graphic_components.hpp>

static constexpr auto kSpriteAdd = "sprite";
static constexpr auto kSpriteLoad = "load_sprite";

#define ADD_CMD_MATCH(cmd, key) \
    key_to_cmd_matcher_[static_cast<uint>(igraphicslib::KeyboardKey::key)] = PlayerCommand::cmd;

void PlayerSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    player_ = entities.Create();
    player_.Assign<PlayerTag>();

    event_manager_ = &events;
    events.Subscribe<PlayerTextRequestEvent>(*this);
    events.Subscribe<KeyPressedEvent>(*this);
    events.Subscribe<KeyReleasedEvent>(*this);

    entities.Tracker().TrackEntity(player_.GetId().GetIndex());

    FillPlayerStatesNameMap(&state_name_to_player_state_id_);

    ADD_CMD_MATCH(WALK_LEFT, A)
    ADD_CMD_MATCH(WALK_RIGHT, D)
    ADD_CMD_MATCH(ATTACK_ONE, J)
    ADD_CMD_MATCH(ATTACK_TWO, K)
    ADD_CMD_MATCH(DEATH, S)
    ADD_CMD_MATCH(JUMP, W)
    ADD_CMD_MATCH(FIRE, F)
    ADD_CMD_MATCH(TEXT_INSERT_REQUEST, T)

    //! remove
    events.Emit<PlayerInitiatedEvent>(player_);
    events.Emit<SkinChangeRequest>(std::move(state_name_to_player_state_id_), static_cast<int>(PlayerCommand::IDLE),
                                   "./assets/sprites/orc_berserk.png", player_);
}

void PlayerSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    
    // todo: fix cycle
    entities.Each<PlayerTag>([this, &events](ecs::Entity player, PlayerTag&) {
        for (auto cmd = commands_queue_.front(); !commands_queue_.empty(); commands_queue_.pop_front()) {
            events.Emit<PlayerCommandEvent>(cmd, player);
        }
    });

    // if (changed_state_ != static_cast<int>(PlayerCommand::INVALID)) {
    //     // events.Emit<SpriteSheetStateChangedEvent>(changed_state_,
    //     // *entities.GetEntitiesWithComponents<PlayerTag>().begin());
    //     changed_state_ = static_cast<int>(PlayerCommand::INVALID);
    // }
}

// // todo: move to smth like player_switch_state_handler
// void PlayerSystem::Receive(const PlayerCommandEvent& event) {

//     switch (event.cmd_) {
//         case PlayerCommand::IDLE:
//         case PlayerCommand::WALK_LEFT:
//         case PlayerCommand::WALK_RIGHT:
//         case PlayerCommand::INVALID:
//         case PlayerCommand::ATTACK_ONE:
//         case PlayerCommand::ATTACK_TWO:
//         case PlayerCommand::JUMP:
//             changed_state_ = static_cast<int>(event.cmd_);
//             break;
//         default:
//             break;
//     }
// }

void PlayerSystem::Receive(const KeyPressedEvent& event) {

    PlayerCommand matched_cmd = key_to_cmd_matcher_[static_cast<uint>(event.data_.key)];

    if (matched_cmd != PlayerCommand::INVALID) {                    
        commands_queue_.push_back(matched_cmd);
    }
}

void PlayerSystem::Receive(const KeyReleasedEvent& event) {
    
    PlayerCommand matched_cmd = key_to_cmd_matcher_[static_cast<uint>(event.data_.key)];
    
    if (matched_cmd != PlayerCommand::INVALID) {                    
        commands_queue_.push_back(PlayerCommand::IDLE);
    }
}

void PlayerSystem::Receive(const PlayerTextRequestEvent& event) {
    if (event.cmd_.empty()) {
        return;
    }

    if (event.cmd_[0] != kSpriteAdd && event.cmd_[0] != kSpriteLoad) {
        return;
    }

    if (event.cmd_.size() == 1) {
        logger::Print(kWarning, "Specify what skin do you wanna load after command\n");
        return;
    }

    // todo: refactor
    if (event.cmd_[0] == kSpriteAdd) {

        event_manager_->Emit<SkinChangeRequest>(state_name_to_player_state_id_, static_cast<int>(PlayerCommand::IDLE),
                                                event.cmd_[1], player_);
    } else if (event.cmd_[0] == kSpriteLoad) {
        event_manager_->Emit<SpriteSheetLoadRequest>(event.cmd_[1]);
    }
}

#define ADD_CMD_STR_MATCH(cmd) storage->operator[](#cmd) = static_cast<int>(PlayerCommand::cmd);

void FillPlayerStatesNameMap(std::map<std::string, int>* storage) {
    ADD_CMD_STR_MATCH(IDLE)
    ADD_CMD_STR_MATCH(WALK_LEFT)
    ADD_CMD_STR_MATCH(WALK_RIGHT)
    ADD_CMD_STR_MATCH(ATTACK_ONE)
    ADD_CMD_STR_MATCH(ATTACK_TWO)
    ADD_CMD_STR_MATCH(DEATH)
    ADD_CMD_STR_MATCH(JUMP)
}
