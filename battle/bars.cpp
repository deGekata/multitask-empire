#include <battle/bars.hpp>

#include <components/battle_components.hpp>
#include <components/bot_components.hpp>
#include <components/graphic_components.hpp>

#include <events/renderer_events.hpp>

static constexpr uint32_t kMaxBarsAmount = 2;

static constexpr uint32_t kBarHeight = 250;
static constexpr uint32_t kBarWidth = 500;

static constexpr uint32_t kDefaultBarX = 10;
static constexpr uint32_t kDefaultBarY = 200;

void BarsSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    entities_ = &entities;
    events_ = &events;

    FillBarsStates();
    FillBarsPositions();

    events_->Subscribe<PlayerInitiatedEvent>(*this);
    events_->Subscribe<DamageTakenEvent>(*this);
    events_->Subscribe<ecs::EntityDestroyedEvent>(*this);
}

void BarsSystem::FillBarsStates() {
    state_name_converter_["TEN"] = BarState::PERCENTS_10;
    state_name_converter_["TWENTY"] = BarState::PERCENTS_20;
    state_name_converter_["THIRTY"] = BarState::PERCENTS_30;
    state_name_converter_["FOURTY"] = BarState::PERCENTS_40;
    state_name_converter_["FIFTY"] = BarState::PERCENTS_50;
    state_name_converter_["SIXTY"] = BarState::PERCENTS_60;
    state_name_converter_["SEVENTY"] = BarState::PERCENTS_70;
    state_name_converter_["EIGHTY"] = BarState::PERCENTS_80;
    state_name_converter_["NINETY"] = BarState::PERCENTS_90;
    state_name_converter_["HUNDRED"] = BarState::PERCENTS_100;
}

void BarsSystem::FillBarsPositions() {
    for (uint32_t cur_bars_row = 0, allies_bar_y = kDefaultBarY; cur_bars_row < kMaxBarsAmount;
         cur_bars_row++, allies_bar_y += kBarHeight) {
        Position alli_bar_position{static_cast<double>(kDefaultBarX), static_cast<double>(allies_bar_y)};
        Position enemy_bar_position{static_cast<double>(kDefaultBarX + kBarWidth), static_cast<double>(allies_bar_y)};

        allies_bars_positions_.push_back(alli_bar_position);
        enemies_bars_positions_.push_back(enemy_bar_position);
    }
}

void BarsSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
}

void BarsSystem::Receive(const ecs::EntityDestroyedEvent& event) {
    if (bar_of_entity_.contains(event.entity_)) {
        BarProperties bar_prop = bar_of_entity_[event.entity_];

        Position free_position = *bar_prop.bar_entity.GetComponent<Position>();

        if (bar_prop.is_enemy) {
            enemies_bars_positions_.push_back(free_position);
        } else {
            allies_bars_positions_.push_back(free_position);
        }

        bar_of_entity_.erase(event.entity_);
        bar_prop.bar_entity.Destroy();
    }
}

void BarsSystem::Receive(const PlayerInitiatedEvent& event) {
    ecs::Entity entity_bar = entities_->Create();

    Position bar_position;
    bool is_rotated = false;
    if (event.entity_.HasComponent<BotBehaviour>()) {
        bar_position = enemies_bars_positions_.back();
        enemies_bars_positions_.pop_back();

        is_rotated = true;
    } else {
        bar_position = allies_bars_positions_.back();
        allies_bars_positions_.pop_back();
    }

    entity_bar.AssignFromCopy<Position>(bar_position);
    entity_bar.AssignFromCopy<Rotation>(Rotation{is_rotated});
    entity_bar.Assign<RenderFrameData>(RenderFrameData{0, false});

    events_->Emit<SkinChangeRequest>(state_name_converter_, BarState::PERCENTS_100, "./assets/sprites/hpbar.png",
                                     entity_bar);

    bar_of_entity_[event.entity_] = {entity_bar, is_rotated};
}

void BarsSystem::Receive(const DamageTakenEvent& event) {
    ecs::Entity damaged_entity = event.damaged_entity_;

    if ((damaged_entity.HasComponent<Health>()) && (bar_of_entity_.contains(damaged_entity))) {
        ecs::Entity health_bar = bar_of_entity_[damaged_entity].bar_entity;
        int32_t health_id = static_cast<int32_t>(damaged_entity.GetComponent<Health>()->health_ / 10.0);

        events_->Emit<SpriteSheetStateChangedEvent>(health_id, damaged_entity);
    }
}
