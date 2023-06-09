#ifndef H_QUICK
#define H_QUICK

#include <ecs/config.hpp>
#include <ecs/entity.hpp>
#include <ecs/event.hpp>
#include <ecs/system.hpp>

namespace ecs {

class ECS {
public:
    ECS();

    EventManager events_;
    EntityManager entities_;
    SystemManager systems_;
};

};  // namespace ecs

#endif
