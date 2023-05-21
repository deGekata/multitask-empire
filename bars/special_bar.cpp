#include <bars/special_bar.hpp>

#include <components/battle_components.hpp>
#include <components/bot_components.hpp>
#include <components/graphic_components.hpp>

#include <events/renderer_events.hpp>

static constexpr uint32_t kMaxBarsAmount = 2;

static constexpr uint32_t kBarHeight = 75;
static constexpr uint32_t kBarWidth = 520;

static constexpr uint32_t kDefaultBarX = 165;
static constexpr uint32_t kDefaultBarY = 585;

void SpecialBarSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    events_ = &events;
    entities_ = &entities;

    FillBarsStates();
    FillBarsPositions();

    events_->Subscribe<PlayerInitiatedEvent>(*this);
    events_->Subscribe<ecs::EntityDestroyedEvent>(*this);
}

#define SET_STATE_CONVERTER(name) state_name_converter_[#name] = SpecialBarState::name

void SpecialBarSystem::FillBarsStates() {
    SET_STATE_CONVERTER(FIRST);
    SET_STATE_CONVERTER(SECOND);
    SET_STATE_CONVERTER(THIRD);
    SET_STATE_CONVERTER(FOURTH);
    SET_STATE_CONVERTER(FIFTH);
    SET_STATE_CONVERTER(SIXTH);
    SET_STATE_CONVERTER(SEVENTH);
}

#undef SET_STATE_CONVERTER

void SpecialBarSystem::FillBarsPositions() {
    for (uint32_t cur_bars_row = 0, allies_bar_y = kDefaultBarY; cur_bars_row < kMaxBarsAmount;
         cur_bars_row++, allies_bar_y += kBarHeight) {
        Position alli_bar_position{static_cast<double>(kDefaultBarX), static_cast<double>(allies_bar_y)};
        Position enemy_bar_position{static_cast<double>(kDefaultBarX + kBarWidth), static_cast<double>(allies_bar_y)};

        allies_bars_positions_.push_back(alli_bar_position);
        enemies_bars_positions_.push_back(enemy_bar_position);
    }
}

void SpecialBarSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta) {
    entities.Each<Special>([this, &events](ecs::Entity entity, Special& special) {
        if (bar_of_entity_.contains(entity)) {
            SpecialBarProperties& properties = bar_of_entity_[entity];

            auto cur_state = static_cast<int32_t>(special.experience_ /
                                                  (100.0 / static_cast<double>(SpecialBarState::SEVENTH)));

            if (cur_state != properties.last_state) {
                events.Emit<SpriteSheetStateChangedEvent>(cur_state, properties.bar_entity);

                properties.last_state = cur_state;
            }
        }
    });
}

void SpecialBarSystem::Receive(const PlayerInitiatedEvent& event) {
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

    events_->Emit<SkinChangeRequest>(state_name_converter_, SpecialBarState::FIRST, "./assets/sprites/special.png",
                                     entity_bar);

    bar_of_entity_[event.entity_] = {entity_bar, is_rotated, 0};
}

void SpecialBarSystem::Receive(const ecs::EntityDestroyedEvent& event) {
    if (bar_of_entity_.contains(event.entity_)) {
        SpecialBarProperties bar_prop = bar_of_entity_[event.entity_];

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
