#ifndef H_DISPATCHER
#define H_DISPATCHER

#include <ecs/system.hpp>

class DispatcherSystem : public ecs::System<DispatcherSystem> {
public:
    DispatcherSystem(bool& running_flag);

    void Update(ecs::EntityManager& entities, ecs::EventManager& events, ecs::TimeDelta dt) override;

private:
    bool& running_;
};

#endif
