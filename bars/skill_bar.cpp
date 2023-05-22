#include <bars/skill_bar.hpp>

#include <components/battle_components.hpp>
#include <components/bot_components.hpp>
#include <components/graphic_components.hpp>
#include <components/player_components.hpp>

#include <events/renderer_events.hpp>

static constexpr uint32_t kMaxBarsAmount = 2;

static constexpr uint32_t kBarHeight = 250;
static constexpr uint32_t kBarWidth = 770;

static constexpr uint32_t kDefaultBarX = 350;
static constexpr uint32_t kDefaultBarY = 550;

void SkillBarSystem::Configure(ecs::EntityManager& entities, ecs::EventManager& events) {
    entities_ = &entities;
    events_ = &events;

    FillBarsStates();
    FillBarsPositions();

    events_->Subscribe<PlayerInitiatedEvent>(*this);
    events_->Subscribe<ecs::EntityDestroyedEvent>(*this);
}

void SkillBarSystem::Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) {
    entities.Each<SpecialAbility>([this, &events](ecs::Entity entity, SpecialAbility& ability) {
        if (bar_of_entity_.contains(entity)) {
            SkillBarProperties& props = bar_of_entity_[entity];

            auto bar_state = static_cast<int32_t>(ability.type_);

            if (props.last_bar_state_ != bar_state) {
                events.Emit<SpriteSheetStateChangedEvent>(bar_state, props.bar_entity);

                props.last_bar_state_ = bar_state;
            }
        }
    });
}

void SkillBarSystem::Receive(const ecs::EntityDestroyedEvent& event) {
    if (bar_of_entity_.contains(event.entity_)) {
        SkillBarProperties bar_prop = bar_of_entity_[event.entity_];

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

void SkillBarSystem::Receive(const PlayerInitiatedEvent& event) {
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
    entity_bar.AssignFromCopy<Rotation>(Rotation{false});

    events_->Emit<SkinChangeRequest>(state_name_converter_, 2, "./assets/sprites/skill_bar.png", entity_bar);

    bar_of_entity_[event.entity_] = {entity_bar, 2, is_rotated};
}

void SkillBarSystem::FillBarsStates() {
    state_name_converter_["FIREBALL"] = SkillBarState::FIREBALL_SKILL;
    state_name_converter_["SLIME"] = SkillBarState::SLIME_SKILL;
    state_name_converter_["POTION"] = SkillBarState::POTION_SKILL;
}

void SkillBarSystem::FillBarsPositions() {
    for (uint32_t cur_bars_row = 0, allies_bar_y = kDefaultBarY; cur_bars_row < kMaxBarsAmount;
         cur_bars_row++, allies_bar_y += kBarHeight) {
        Position alli_bar_position{static_cast<double>(kDefaultBarX), static_cast<double>(allies_bar_y)};
        Position enemy_bar_position{static_cast<double>(kDefaultBarX + kBarWidth), static_cast<double>(allies_bar_y)};

        allies_bars_positions_.push_back(alli_bar_position);
        enemies_bars_positions_.push_back(enemy_bar_position);
    }
}
