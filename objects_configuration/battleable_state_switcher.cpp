#include <objects_configuration/battleable_state_switcher.hpp>

#include <components/graphic_components.hpp>
#include <components/movement_components.hpp>

void BattleAbleStateSwitchSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    
    events.Subscribe<ActionCommandRequestEvent>(*this);
    events.Subscribe<StateRenderedEvent>(*this);
    events_ = &events;
}

void BattleAbleStateSwitchSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {

    entities.Each<PBattleAbleAttributes>(
    [&entities, &events](ecs::Entity entity, PBattleAbleAttributes& attrs) {
        auto anim_data = entity.GetComponent<ObjectAnimationData>().Get();

        if(attrs.next_active_state_ != PBattleAbleAttributes::kInvalidState && !anim_data->is_one_shot_) {

            // if(attrs.cur_active_state_ == static_cast<int>(ActionCommandType::Jump) && attrs.next_active_state_ == static_cast<int>(ActionCommandType::Jump)) {
            //     attrs.next_active_state_ = static_cast<int>(ActionCommandType::Idle);
            // }

            ecs::Entity action = entities.Create();

            ActionCommand cmd = {.type_ = static_cast<ActionCommandType>(attrs.next_active_state_)};
            action.Assign<ActionCommand>(cmd);
            
            events.Emit<ActionCommandEvent>(action, entity);
            entities.Destroy(action.GetId());

            events.Emit<SpriteSheetStateChangedEvent>(attrs.next_active_state_, entity, true); 

            attrs.next_active_state_ = PBattleAbleAttributes::kInvalidState;
        }

        if(attrs.next_passive_state_ != PBattleAbleAttributes::kInvalidState) {
        
            ecs::Entity action = entities.Create();

            ActionCommand cmd = {.type_ = static_cast<ActionCommandType>(attrs.next_passive_state_)};
            action.Assign<ActionCommand>(cmd);

            events.Emit<ActionCommandEvent>(action, entity);
            
            entities.Destroy(action.GetId());

            if(attrs.next_active_state_ == PBattleAbleAttributes::kInvalidState && !anim_data->is_one_shot_) {
                // todo: fix
                // todo: remove
                if(attrs.next_passive_state_ == static_cast<int>(ActionCommandType::RunLeft)) {
                    entity.GetComponent<Rotation>()->is_flipped_ = true;
                    events.Emit<SpriteSheetStateChangedEvent>(static_cast<int>(ActionCommandType::RunRight), entity); 
                }
                else if(attrs.next_passive_state_ == static_cast<int>(ActionCommandType::RunRight)) {
                    entity.GetComponent<Rotation>()->is_flipped_ = false;
                    events.Emit<SpriteSheetStateChangedEvent>(static_cast<int>(ActionCommandType::RunRight), entity); 
                }
                else if(cmd.type_ == ActionCommandType::StopRunningLeft || cmd.type_ == ActionCommandType::StopRunningRight) {
                    events.Emit<SpriteSheetStateChangedEvent>(static_cast<int>(ActionCommandType::Idle), entity); 
                }
                else if(cmd.type_ != ActionCommandType::Special && cmd.type_ != ActionCommandType::Block) {
                    events.Emit<SpriteSheetStateChangedEvent>(attrs.next_passive_state_, entity); 
                }
            }

            attrs.cur_passive_state_ = attrs.next_passive_state_;
            if(cmd.type_ == ActionCommandType::StopRunningLeft || cmd.type_ == ActionCommandType::StopRunningRight) {
                attrs.cur_passive_state_ = static_cast<int>(ActionCommandType::Idle);
            }
            attrs.next_passive_state_ = PBattleAbleAttributes::kInvalidState;
        }
    });
}

void BattleAbleStateSwitchSystem::Receive(const ActionCommandRequestEvent& event) {
    ecs::Entity action = event.action_;
    ecs::Entity obj_entity = event.obj_entity_;

    if(obj_entity.HasComponent<ObjectAnimationData>() == 0) {
        return;
    }

    auto cmd = action.GetComponent<ActionCommand>().Get();

    auto attrs = obj_entity.GetComponent<PBattleAbleAttributes>().Get();
    // auto anim_data = obj_entity.GetComponent<ObjectAnimationData>().Get();

    if(cmd->type_ == ActionCommandType::Idle || cmd->type_ == ActionCommandType::Block || cmd->type_ == ActionCommandType::Special || 
       cmd->type_ == ActionCommandType::Block || cmd->type_ == ActionCommandType::RunLeft ||
       cmd->type_ == ActionCommandType::RunRight || cmd->type_ == ActionCommandType::StopRunningLeft ||
       cmd->type_ == ActionCommandType::StopRunningRight) {
        
        attrs->next_passive_state_ = static_cast<int>(cmd->type_);
        
        // if(!anim_data->is_one_shot_) {
        //     events_->Emit<SpriteSheetStateChangedEvent>(attrs->next_active_state_, true);
        //     events_->Emit<ActionCommandEvent>(action, obj_entity);
        // }
    }
    else {
        // if(cmd->type_ == ActionCommandType::Attack) {
        //     // todo:
        // }Rotation
        attrs->next_active_state_ = static_cast<int>(cmd->type_);

        if(cmd->type_ == ActionCommandType::Attack) {

            auto offset = action.GetComponent<AttackId>().Get();
            attrs->next_active_state_ += static_cast<int>(offset->id_);
        }
    }
}

void BattleAbleStateSwitchSystem::Receive(const StateRenderedEvent& event) {
    ecs::Entity entity = event.entity_;

    if(!entity.HasComponent<PBattleAbleAttributes>()) return;

    auto attrs = entity.GetComponent<PBattleAbleAttributes>().Get();
    
    if(attrs->next_active_state_ == PBattleAbleAttributes::kInvalidState && attrs->next_passive_state_ == PBattleAbleAttributes::kInvalidState) {
        attrs->next_passive_state_ = attrs->cur_passive_state_;
    }
}
