#ifndef H_SHIELD_BAR
#define H_SHIELD_BAR

#include <ecs/quick.hpp>

#include <components/movement_components.hpp>

#include <events/player_events.hpp>

class ShieldBarSystem : public ecs::System<ShieldBarSystem>, public ecs::Reciever<ShieldBarSystem> {
    enum ShieldBarState : int {
        PERCENTS_0,
        PERCENTS_10,
        PERCENTS_20,
        PERCENTS_30,
        PERCENTS_40,
        PERCENTS_50,
        PERCENTS_60,
        PERCENTS_70,
        PERCENTS_80,
        PERCENTS_90,
        PERCENTS_100,
        STATES_AMOUNT
    };

    enum ShieldStates: int {
        DEACTIVATED,
        ACTIVATED
    };

    struct ShieldBarProperties {
        ecs::Entity bar_entity;
        ecs::Entity shield_state_entity;

        bool is_enemy;
        int32_t last_bar_state;
        bool last_shield_state;
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

    std::map<std::string, int> bar_name_converter_;
    std::map<std::string, int> shield_name_converter_;

    std::map<ecs::Entity, ShieldBarProperties> bar_of_entity_;

    ecs::EntityManager* entities_;
    ecs::EventManager* events_;
};

#endif
