#include <battle/block.hpp>

#include <components/battle_components.hpp>

void BlockSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
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
    ecs::Entity entity = event.entity_;
    if ((event.cmd_.type_ == PlayerCommandType::Block) && (!entity.HasComponent<BlockedTag>())) {
        entity.Assign<BlockedTag>();
    } else if (entity.HasComponent<BlockedTag>()) {
        entity.Remove<BlockedTag>();
    }
}
