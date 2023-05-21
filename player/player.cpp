#include <player/player.hpp>

#include <events/player_events.hpp>
#include <events/renderer_events.hpp>

#include <events/action_events.hpp>

#include <components/movement_components.hpp>
#include <components/player_components.hpp>
#include <components/graphic_components.hpp>

static constexpr auto kSpriteAdd = "sprite";
static constexpr auto kSpriteLoad = "load_sprite";

#define ADD_CMD_MATCH(cmd, key) \
    key_to_cmd_matcher_[igraphicslib::KeyboardKey::key] = PlayerCommandType::cmd;

#define ADD_ACTION_MATCH(cmd, key) \
    key_to_cmd_matcher_[igraphicslib::KeyboardKey::key] = PlayerCommandType::Action; \
    key_to_action_matcher_[igraphicslib::KeyboardKey::key] = ActionCommandType::cmd;

void PlayerSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    player_ = entities.Create();
    player_.Assign<PlayerTag>();

    event_manager_ = &events;

    events.Subscribe<PlayerInitiatedEvent>(*this);
    events.Subscribe<PlayerTextRequestEvent>(*this);
    events.Subscribe<KeyPressedEvent>(*this);
    events.Subscribe<KeyReleasedEvent>(*this);

    entities.Tracker().TrackEntity(player_.GetId().GetIndex());

    ADD_ACTION_MATCH(RunRight, D)
    ADD_ACTION_MATCH(RunLeft, A)
    ADD_ACTION_MATCH(Jump, W)
    ADD_ACTION_MATCH(Attack, H)
    ADD_ACTION_MATCH(Attack, J)
    ADD_ACTION_MATCH(Attack, K)
    ADD_ACTION_MATCH(Attack, L)
    ADD_ACTION_MATCH(Special, F)
    ADD_ACTION_MATCH(Block, B)

    ADD_CMD_MATCH(TextInsertRequest, T)

    //! remove
    events.Emit<PlayerInitiatedEvent>(player_);
    events.Emit<BattleAbleConfigChangeRequest>("./orc.wtf", player_);
}

// todo: refactor (0)___(0)

void PlayerSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {

    while(!commands_queue_.empty()){
        auto req = commands_queue_.front();
        commands_queue_.pop_back();
        
        if(key_to_cmd_matcher_.count(req.key) > 0) {
            ecs::Entity cmd_ent = entities.Create();

            auto type = key_to_cmd_matcher_[req.key];

            PlayerCommand cmd = {.type_ = type};
            cmd_ent.Assign<PlayerCommand>(cmd);

            if(type == PlayerCommandType::Action) {

                auto attr_data = player_.GetComponent<PBattleAbleAttributes>().Get();

                ActionCommandType action = key_to_action_matcher_[req.key];

                ActionCommand action_cmd = {.type_ = action};
                cmd_ent.Assign<ActionCommand>(action_cmd);

                if(action == ActionCommandType::Attack) {
                    
                    int attack_id = -1;

                    for(uint n_attack = 0; n_attack < attr_data->attr_->attacks_.size(); n_attack++) {
                        if(attr_data->attr_->attacks_[n_attack].binded_key_ == req.key) {
                            attack_id = static_cast<int>(n_attack); 
                            break;
                        }
                    }

                    if(attack_id == -1) {
                        logger::Print(kError, "unable to find attack, binded on key {}\n", static_cast<int>(req.key));
                    }

                    AttackId attack_id_cmp = {.id_ = static_cast<uint>(attack_id)};
                    cmd_ent.Assign<AttackId>(attack_id_cmp);
                }

                events.Emit<ActionCommandRequestEvent>(cmd_ent, player_);
            }

            events.Emit<PlayerCommandEvent>(cmd_ent, player_);
            entities.Destroy(cmd_ent.GetId());
        }
    }

    if(on_release_queue_.empty()){
        return;
    }

    if(!on_release_queue_.empty()){

        while(!on_release_queue_.empty()){
            auto req = on_release_queue_.front();
            on_release_queue_.pop_back();

            //? extra logic
            if(key_to_cmd_matcher_.count(req.key) > 0 && key_to_cmd_matcher_[req.key] == PlayerCommandType::Action) {
                ActionCommandType action = key_to_action_matcher_[req.key];

                if(action == ActionCommandType::RunLeft) {
                    
                    ecs::Entity cmd_ent = entities.Create();
                    
                    PlayerCommand cmd = {.type_ = PlayerCommandType::Action};
                    cmd_ent.Assign<PlayerCommand>(cmd);

                    ActionCommand action_cmd = {.type_ = ActionCommandType::StopRunningLeft};
                    cmd_ent.Assign<ActionCommand>(action_cmd);

                    events.Emit<ActionCommandRequestEvent>(cmd_ent, player_);
                    events.Emit<PlayerCommandEvent>(cmd_ent, player_);
                }
                if(action == ActionCommandType::RunRight) {
                    
                    ecs::Entity cmd_ent = entities.Create();
                    PlayerCommand cmd = {.type_ = PlayerCommandType::Action};
                    cmd_ent.Assign<PlayerCommand>(cmd);

                    ActionCommand action_cmd = {.type_ = ActionCommandType::StopRunningRight};
                    cmd_ent.Assign<ActionCommand>(action_cmd);

                    events.Emit<ActionCommandRequestEvent>(cmd_ent, player_);
                    events.Emit<PlayerCommandEvent>(cmd_ent, player_);
                }
            }
        }
    }
}

void PlayerSystem::Receive(const KeyPressedEvent& event) {

    commands_queue_.push_back(event.data_);
}

void PlayerSystem::Receive(const KeyReleasedEvent& event) {

    on_release_queue_.push_back(event.data_);
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

        event_manager_->Emit<BattleAbleConfigChangeRequest>(event.cmd_[1], player_);
    } else if (event.cmd_[0] == kSpriteLoad) {
        event_manager_->Emit<SpriteSheetLoadRequest>(event.cmd_[1]);
    }
}

void PlayerSystem::Receive(const PlayerInitiatedEvent&) {
}
