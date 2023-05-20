#include <battle/block.hpp>

#include <components/battle_components.hpp>

static constexpr double kBlockMaxDurability = 50.0;
static constexpr double kDurabilityIncreasePerDt = 1e-5;

void BlockSystem::Configure(ecs::EntityManager&, ecs::EventManager& events) {
    events.Subscribe<PlayerCommandEvent>(*this);
    events.Subscribe<PlayerInitiatedEvent>(*this);
}

void BlockSystem::Update(ecs::EntityManager& entities, ecs::EventManager&, ecs::TimeDelta dt) {
    std::vector<ecs::Entity> entities_to_erase_block;

    entities.Each<BlockReserve>([dt, &entities_to_erase_block](ecs::Entity entity, BlockReserve& block) {
        if (entity.HasComponent<BlockedTag>()) {
            if (block.durability_ <= 0.0) {
                entities_to_erase_block.push_back(entity);
            }
        } else {
            block.durability_ = std::min(kBlockMaxDurability, block.durability_ + kDurabilityIncreasePerDt * dt);
        }
    });

    for (auto& it : entities_to_erase_block) {
        it.Remove<BlockedTag>();
    }
}

void BlockSystem::Receive(const PlayerCommandEvent& event) {
    ecs::Entity entity = event.entity_;
    if ((event.cmd_ == PlayerCommand::BLOCK) && (!entity.HasComponent<BlockedTag>())) {
        entity.Assign<BlockedTag>();
    } else if (entity.HasComponent<BlockedTag>()) {
        entity.Remove<BlockedTag>();
    }
}

void BlockSystem::Receive(const PlayerInitiatedEvent& event) {
    ecs::Entity player = event.entity_;

    player.Assign<BlockReserve>(BlockReserve{0.0});
}
