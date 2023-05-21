#include <battle/block.hpp>

#include <components/battle_components.hpp>

void BlockSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
}

void BlockSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta dt) {
    std::vector<ecs::Entity> entities_to_erase_block;

    entities.Each<BlockedTag>([dt, &entities_to_erase_block](ecs::Entity entity, BlockedTag& block) {
        block.remain_time_ -= dt;

        if (block.remain_time_ <= 0) {
            entities_to_erase_block.push_back(entity);
        }
    });

    for (auto& it : entities_to_erase_block) {
        it.Remove<BlockedTag>();
    }
}

void BlockSystem::Receive(const PlayerCommandEvent& event) {
    
    ecs::Entity cmd_ent = event.cmd_;

    auto cmd_type = cmd_ent.GetComponent<PlayerCommand>().Get();
    if(cmd_type->type_ != PlayerCommandType::Action) return;

    ecs::Entity player_entity = event.entity_;
    auto action_type = cmd_ent.GetComponent<ActionCommand>().Get();

    if ((action_type->type_ == ActionCommandType::Block) && (!player_entity.HasComponent<BlockedTag>())) {
        player_entity.Assign<BlockedTag>();
    } else if (player_entity.HasComponent<BlockedTag>()) {
        player_entity.Remove<BlockedTag>();
    }
}
