#include <bars/shield_bar.hpp>

#include <components/battle_components.hpp>
#include <components/bot_components.hpp>
#include <components/graphic_components.hpp>
#include <components/player_components.hpp>

#include <events/renderer_events.hpp>

static constexpr double kBlockMaxDurability = 50.0;

static constexpr uint32_t kMaxBarsAmount = 2;

static constexpr uint32_t kBarHeight = 250;
static constexpr uint32_t kBarWidth = 1445;

static constexpr uint32_t kDefaultBarX = 165;
static constexpr uint32_t kDefaultBarY = 665;

static constexpr double kShieldStateShiftX = -10.0;
static constexpr double kShieldStateShiftY = -10.0;

static constexpr double kShieldStateInversedDelta = 100.0;

void ShieldBarSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    entities_ = &entities;
    events_ = &events;

    FillBarsStates();
    FillBarsPositions();

    events_->Subscribe<ecs::EntityDestroyedEvent>(*this);
    events_->Subscribe<PlayerInitiatedEvent>(*this);
}

void ShieldBarSystem::FillBarsStates() {
    // Cow counter reference
    bar_name_converter_["ZERO"] = ShieldBarState::PERCENTS_0;
    bar_name_converter_["TEN"] = ShieldBarState::PERCENTS_10;
    bar_name_converter_["TWENTY"] = ShieldBarState::PERCENTS_20;
    bar_name_converter_["THIRTY"] = ShieldBarState::PERCENTS_30;
    bar_name_converter_["FOURTY"] = ShieldBarState::PERCENTS_40;
    bar_name_converter_["FIFTY"] = ShieldBarState::PERCENTS_50;
    bar_name_converter_["SIXTY"] = ShieldBarState::PERCENTS_60;
    bar_name_converter_["SEVENTY"] = ShieldBarState::PERCENTS_70;
    bar_name_converter_["EIGHTY"] = ShieldBarState::PERCENTS_80;
    bar_name_converter_["NINETY"] = ShieldBarState::PERCENTS_90;
    bar_name_converter_["HUNDRED"] = ShieldBarState::PERCENTS_100;

    shield_name_converter_["DEACTIVATED"] = ShieldStates::DEACTIVATED;
    shield_name_converter_["ACTIVATED"] = ShieldStates::ACTIVATED;
}

void ShieldBarSystem::FillBarsPositions() {
    for (uint32_t cur_bars_row = 0, allies_bar_y = kDefaultBarY; cur_bars_row < kMaxBarsAmount;
         cur_bars_row++, allies_bar_y += kBarHeight) {
        Position alli_bar_position{static_cast<double>(kDefaultBarX), static_cast<double>(allies_bar_y)};
        Position enemy_bar_position{static_cast<double>(kDefaultBarX + kBarWidth), static_cast<double>(allies_bar_y)};

        allies_bars_positions_.push_back(alli_bar_position);
        enemies_bars_positions_.push_back(enemy_bar_position);
    }
}

void ShieldBarSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    entities.Each<BlockReserve>([this, &events](ecs::Entity entity, BlockReserve& block) {
        if (bar_of_entity_.contains(entity)) {
            ShieldBarProperties& props = bar_of_entity_[entity];

            auto bar_state = static_cast<int32_t>(
                block.durability_ / (kBlockMaxDurability / static_cast<double>(ShieldBarState::PERCENTS_100)));
            bool shield_state = entity.HasComponent<BlockedTag>();

            if (props.last_bar_state != bar_state) {
                events.Emit<SpriteSheetStateChangedEvent>(bar_state, props.bar_entity);

                props.last_bar_state = bar_state;
            }

            if (props.last_shield_state != shield_state) {
                events.Emit<SpriteSheetStateChangedEvent>(shield_state ? 1 : 0, props.shield_state_entity);

                props.last_shield_state = shield_state;
            }
        }
    });
}

void ShieldBarSystem::Receive(const ecs::EntityDestroyedEvent& event) {
    if (bar_of_entity_.contains(event.entity_)) {
        ShieldBarProperties bar_prop = bar_of_entity_[event.entity_];

        Position free_position = *bar_prop.bar_entity.GetComponent<Position>();

        if (bar_prop.is_enemy) {
            enemies_bars_positions_.push_back(free_position);
        } else {
            allies_bars_positions_.push_back(free_position);
        }

        bar_of_entity_.erase(event.entity_);

        bar_prop.bar_entity.Destroy();
        bar_prop.shield_state_entity.Destroy();
    }
}

void ShieldBarSystem::Receive(const PlayerInitiatedEvent& event) {
    ecs::Entity entity_bar = entities_->Create();
    ecs::Entity entity_shield_state = entities_->Create();

    Position bar_position;
    Position state_position;

    bool is_rotated = false;
    if (event.entity_.HasComponent<BotBehaviour>()) {
        bar_position = enemies_bars_positions_.back();
        enemies_bars_positions_.pop_back();

        state_position = bar_position;
        state_position.x_ += kShieldStateInversedDelta;
        state_position.x_ -= kShieldStateShiftX;
        state_position.y_ += kShieldStateShiftY;

        is_rotated = true;
    } else {
        bar_position = allies_bars_positions_.back();
        allies_bars_positions_.pop_back();

        state_position = bar_position;
        state_position.x_ += kShieldStateShiftX;
        state_position.y_ += kShieldStateShiftY;
    }

    entity_bar.AssignFromCopy<Position>(bar_position);
    entity_bar.AssignFromCopy<Rotation>(Rotation{is_rotated});

    entity_shield_state.AssignFromCopy<Position>(state_position);
    entity_shield_state.AssignFromCopy<Rotation>(Rotation{is_rotated});

    events_->Emit<SkinChangeRequest>(bar_name_converter_, ShieldBarState::PERCENTS_0, "./assets/sprites/shield_bar.png",
                                     entity_bar);
    events_->Emit<SkinChangeRequest>(shield_name_converter_, ShieldStates::DEACTIVATED,
                                     "./assets/sprites/shield_states.png", entity_shield_state);

    bar_of_entity_[event.entity_] = {entity_bar, entity_shield_state, is_rotated, 0, false};
}
