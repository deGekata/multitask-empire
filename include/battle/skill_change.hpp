#ifndef H_SKILL_CHANGE
#define H_SKILL_CHANGE

#include <ecs/quick.hpp>

#include <events/player_events.hpp>

class SkillChangeSystem : public ecs::System<SkillChangeSystem>, public ecs::Receiver<SkillChangeSystem> {
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    void Receive(const KeyPressedEvent& event);
    void Receive(const PlayerInitiatedEvent& event);

private:
    ecs::Entity player_;
};

#endif
