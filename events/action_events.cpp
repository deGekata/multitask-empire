#include <events/action_events.hpp>

void EmitNonArgsAction(ecs::Entity obj_entity, ActionCommandType type, ecs::EntityManager& entities, ecs::EventManager& events) {

    ecs::Entity action = entities.Create();

    ActionCommand cmd = {.type_ = type};
    action.Assign<ActionCommand>(cmd);
    events.Emit<ActionCommandRequestEvent>(action, obj_entity);
    entities.Destroy(action.GetId());
}

void EmitNonAttackAction(ecs::Entity obj_entity, uint attack_id, ecs::EntityManager& entities, ecs::EventManager& events) {
    ecs::Entity action = entities.Create();

    ActionCommand cmd = {.type_ = ActionCommandType::Attack};
    action.Assign<ActionCommand>(cmd);

    AttackId attack = {.id_ = attack_id};
    action.Assign<AttackId>(attack);

    events.Emit<ActionCommandRequestEvent>(action, obj_entity);
    entities.Destroy(action.GetId());
}
