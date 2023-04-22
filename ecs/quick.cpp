#include <ecs/quick.hpp>

namespace ecs {
ECS::ECS() : events_(), entities_(events_), systems_(entities_, events_) {
}
};  // namespace ecs
