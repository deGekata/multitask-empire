#ifndef H_CONFIG
#define H_CONFIG

#include <cstdint>

#include <bitset>
#include <chrono>

namespace ecs {

constexpr uint64_t kMaxComponents = 128;
constexpr uint64_t kMaxEntities   = 8 * 1024;
constexpr uint64_t kMaxEvents     = 8 * 1024;

// TODO std::vector<bool>
using ComponentMask = std::bitset<kMaxComponents>;

// In nanoseconds
using TimeDelta = double;

};  // namespace ecs

#endif
