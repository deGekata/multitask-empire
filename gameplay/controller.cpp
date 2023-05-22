#include <gameplay/controller.hpp>

#include <events/bot_events.hpp>
#include <events/player_events.hpp>
#include <events/action_events.hpp>

#include <components/battle_components.hpp>
#include <components/bot_components.hpp>
#include <components/player_components.hpp>
#include <components/collision_components.hpp>

#include <components/graphic_components.hpp>
metrics::TimeStorage ControllerSystem::update_timestamp_ = metrics::CurTime();
metrics::TimeStorage ControllerSystem::block_timestamp_ = metrics::CurTime();

ControllerSystem::ControllerSystem() : current_state_(GameState::Init) {
}

void ControllerSystem::Configure(ecs::EntityManager&, ecs::EventManager&) {
}

void ControllerSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
    uint32_t enemies_counter = 0;
    entities.Each<BotBehaviour>([&enemies_counter](ecs::Entity, BotBehaviour&) { enemies_counter++; });

    if (enemies_counter == 0) {
        SwitchGameState(entities, events, dt);
    }
}

void ControllerSystem::KnightBehaviour(ecs::Entity current, ecs::EntityManager& entities, ecs::EventManager& events,
                                       ecs::TimeDelta dt) {

    if(current.HasComponent<PBattleAbleAttributes>() == false) {
        return;
    }

    auto attrs = current.GetComponent<PBattleAbleAttributes>().Get();
    
    ecs::Entity player = *(entities.GetEntitiesWithComponents<PlayerTag>().begin());

    if(attrs->cur_active_state_ == PBattleAbleAttributes::kInvalidState){
        
        int cur_state = attrs->cur_passive_state_;

        auto player_pos = player.GetComponent<Position>().Get();
        auto bot_pos    = current.GetComponent<Position>().Get();

        if(player_pos->x_ > bot_pos->x_ && current.GetComponent<Rotation>()->is_flipped_) {
            current.GetComponent<Rotation>()->is_flipped_ = false;
        }
        if(player_pos->x_ < bot_pos->x_ && !current.GetComponent<Rotation>()->is_flipped_) {
            current.GetComponent<Rotation>()->is_flipped_ = true;            
        }

        if(cur_state == static_cast<int>(ActionCommandType::RunLeft) || cur_state == static_cast<int>(ActionCommandType::RunRight)) {
            
            bool is_left = attrs->cur_passive_state_ == static_cast<int>(ActionCommandType::RunLeft);

            // todo: remove
            if(player_pos->x_ < bot_pos->x_ && player_pos->x_ + player.GetComponent<HitBox>()->width_ - 70> bot_pos->x_ - (current.GetComponent<AttackDistance>()->distance_)) {
                EmitNonArgsAction(current, ActionCommandType::StopRunningLeft, entities, events);
            }
            else if(player_pos->x_ > bot_pos->x_ && player_pos->x_ - player.GetComponent<HitBox>()->width_ + 70 < bot_pos->x_ + (current.GetComponent<AttackDistance>()->distance_)){
                EmitNonArgsAction(current, ActionCommandType::StopRunningRight, entities, events);
            }
        }

        else if(cur_state == static_cast<int>(ActionCommandType::Idle) && !metrics::CheckDuration(update_timestamp_, 3)) {

            if(current.HasComponent<BlockedTag>()) {
                current.Remove<BlockedTag>();
            }

            if(player_pos->x_ + player.GetComponent<HitBox>()->width_ < bot_pos->x_ - current.GetComponent<AttackDistance>()->distance_) {
                EmitNonArgsAction(current, ActionCommandType::RunLeft, entities, events);
            }
            else if(player_pos->x_ - player.GetComponent<HitBox>()->width_  > bot_pos->x_ + current.GetComponent<AttackDistance>()->distance_) {
                EmitNonArgsAction(current, ActionCommandType::RunRight, entities, events);
            }
            
            else {
                EmitNonAttackAction(current, 0, entities, events);
            }
        }

        else if(cur_state == static_cast<int>(ActionCommandType::Idle) && !metrics::CheckDuration(block_timestamp_, 1.5)) {
            if(!current.HasComponent<BlockedTag>()) {
                current.Assign<BlockedTag>();
            }
        }
    }
}

void ControllerSystem::SwitchGameState(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    switch (current_state_) {
        case GameState::Init:{
            entities.Each<PlayerTag>([](ecs::Entity entity, PlayerTag&) {
                entity.Assign<SpecialAbility>(SpecialAbility{SpecialAbility::Type::Potion});
            });


            ecs::Entity backgr = entities.Create();

            BackgroundR comp = {.texture_ = igraphicslib::Texture("background.png")};
            backgr.Assign<BackgroundR>(comp);

            current_state_ = GameState::Knight1;
            break;
        }

        case GameState::Knight1: {
            events.Emit<SpawnBotEvent>(Position{500, 0}, "./knight.wtf",
                                       &ControllerSystem::KnightBehaviour);

            auto back_entity = *(entities.GetEntitiesWithComponents<BackgroundR>().begin());
            back_entity.Remove<BackgroundR>();
            BackgroundR comp = {.texture_ = igraphicslib::Texture("background.png")};
            back_entity.Assign<BackgroundR>(comp);

            current_state_ = GameState::Knight2;
            break;
        }

        case GameState::Knight2: {
            events.Emit<SpawnBotEvent>(Position{500, 0}, "./father_knight.wtf",
                                       &ControllerSystem::KnightBehaviour);

            auto back_entity = *(entities.GetEntitiesWithComponents<BackgroundR>().begin());
            back_entity.Remove<BackgroundR>();
            BackgroundR comp = {.texture_ = igraphicslib::Texture("background1.png")};
            back_entity.Assign<BackgroundR>(comp);

            current_state_ = GameState::Knight3;
            break;
        }

        case GameState::Knight3: {
            events.Emit<SpawnBotEvent>(Position{500, 0}, "./mother_knight.wtf",
                                       &ControllerSystem::KnightBehaviour);

            auto back_entity = *(entities.GetEntitiesWithComponents<BackgroundR>().begin());
            back_entity.Remove<BackgroundR>();
            BackgroundR comp = {.texture_ = igraphicslib::Texture("background.png")};
            back_entity.Assign<BackgroundR>(comp);

            current_state_ = GameState::Final;
            break;
        }

        case GameState::Final:{
            break;
        }
    }
}
