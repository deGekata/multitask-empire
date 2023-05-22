#ifndef H_SPECIAL_BAR
#define H_SPECIAL_BAR

#include <ecs/quick.hpp>

#include <components/bars_components.hpp>
#include <components/movement_components.hpp>

#include <events/player_events.hpp>

class SpecialBarSystem : public ecs::System<SpecialBarSystem>, public ecs::Receiver<SpecialBarSystem> {
    enum SpecialBarState : int { FIRST, SECOND, THIRD, FOURTH, FIFTH, SIXTH, SEVENTH, STATES_AMOUNT };

    struct SpecialBarProperties {
        ecs::Entity bar_entity;

        bool is_enemy;
        int32_t last_state;
    };

public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const PlayerInitiatedEvent& event);
    void Receive(const ecs::EntityDestroyedEvent& event);

private:
    void FillBarsStates();
    void FillBarsPositions();

    std::vector<Position> allies_bars_positions_;
    std::vector<Position> enemies_bars_positions_;

    std::map<std::string, int> state_name_converter_;

    std::map<ecs::Entity, SpecialBarProperties> bar_of_entity_;

    ecs::EventManager* events_;
    ecs::EntityManager* entities_;
};

#endif
