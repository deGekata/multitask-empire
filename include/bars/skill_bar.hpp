#ifndef H_SKILL_BAR
#define H_SKILL_BAR

#include <ecs/quick.hpp>

#include <components/movement_components.hpp>

#include <events/player_events.hpp>

class SkillBarSystem : public ecs::System<SkillBarSystem>, public ecs::Receiver<SkillBarSystem> {
    enum SkillBarState : int { FIREBALL_SKILL, SLIME_SKILL, POTION_SKILL };

    struct SkillBarProperties {
        ecs::Entity bar_entity;

        int32_t last_bar_state_;
        bool is_enemy;
    };

public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const ecs::EntityDestroyedEvent& event);
    void Receive(const PlayerInitiatedEvent& event);

private:
    void FillBarsStates();
    void FillBarsPositions();

    std::vector<Position> allies_bars_positions_;
    std::vector<Position> enemies_bars_positions_;

    std::map<std::string, int> state_name_converter_;

    std::map<ecs::Entity, SkillBarProperties> bar_of_entity_;

    ecs::EntityManager* entities_;
    ecs::EventManager* events_;
};

#endif
