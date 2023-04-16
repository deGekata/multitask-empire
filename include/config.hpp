#pragma once

#include <cstdint>

#include <bitset>

namespace ecs {
constexpr uint64_t MAX_COMPONENTS = 128;

// TODO std::vector<bool>
using ComponentMask = std::bitset<MAX_COMPONENTS>;

// TODO std::chrono??
using TimeDelta = double;

};  // namespace ecs