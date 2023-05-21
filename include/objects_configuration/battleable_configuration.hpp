#ifndef BATTLEABLE_CONFIG_H
#define BATTLEABLE_CONFIG_H

#include <queue>

#include <ecs/system.hpp>

#include <events/renderer_events.hpp>

#include <parsing/character_config_parser.hpp>

class BattleAbleObjectsConfigSystem : public ecs::System<BattleAbleObjectsConfigSystem>, public ecs::Reciever<BattleAbleObjectsConfigSystem>{
public:
    void Configure(ecs::EntityManager& entities, ecs::EventManager& events) override;
    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

    // parses xml file at path xml_path, creates component Spritesheet
    void LoadConfig(const std::string& path, ecs::EntityManager& entities);

    void Receive(const BattleAbleConfigChangeRequest& event);
    void Receive(const BattleAbleConfigLoadRequest& event);

    void ChangeConfiguration(ecs::Entity entity, BattleAbleAttributes& attrs, ecs::EventManager& events);
private:
    CharacterConfigParser   parser_;
    std::queue<BattleAbleConfigChangeRequest> requests_;
    std::queue<BattleAbleConfigLoadRequest> load_requests_;
};

#endif // BATTLEABLE_CONFIG_H
