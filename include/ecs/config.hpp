#ifndef H_CONFIG
#define H_CONFIG

#include <cstdint>

#include <bitset>
#include <chrono>

namespace ecs {

constexpr uint64_t MAX_COMPONENTS = 128;
constexpr uint64_t MAX_ENTITIES   = 8 * 1024;
constexpr uint64_t MAX_EVENTS     = 8 * 1024;

// TODO std::vector<bool>
using ComponentMask = std::bitset<MAX_COMPONENTS>;

// In seconds
using TimeDelta = int64_t;

};  // namespace ecs

#endif
