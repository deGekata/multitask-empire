#ifndef BOT_AI_HPP
#define BOT_AI_HPP

#include <ecs/entity.hpp>

class BotAI {
public:
    virtual void Update(ecs::Entity bot, ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) = 0;
};

// class FullMeleeAI : public BotAI {
// public:
//     void Update(ecs::Entity bot, ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

// private:
//     metrics::TimeStorage position_check_timestamp_;
// };

#endif // BOT_AI_HPP
