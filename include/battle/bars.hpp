#ifndef H_BARS
#define H_BARS

#include <ecs/quick.hpp>

#include <components/movement_components.hpp>

#include <events/battle_events.hpp>
#include <events/player_events.hpp>

class BarsSystem : public ecs::System<BarsSystem>, public ecs::Reciever<BarsSystem> {
    enum BarState : int {
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
    };

    struct BarProperties {
        ecs::Entity bar_entity;
        bool is_enemy;        
    };

public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const ecs::EntityDestroyedEvent& event);
    void Receive(const PlayerInitiatedEvent& event);
    void Receive(const DamageTakenEvent& event);

private:
    void FillBarsStates();
    void FillBarsPositions();

    std::vector<Position> allies_bars_positions_;
    std::vector<Position> enemies_bars_positions_;

    std::map<std::string, int> state_name_converter_;

    std::map<ecs::Entity, BarProperties> bar_of_entity_;

    ecs::EntityManager* entities_;
    ecs::EventManager* events_;
};

#endif
