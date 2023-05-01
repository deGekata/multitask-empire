#ifndef LOGGER_COLORS_H
#define LOGGER_COLORS_H

#include <cstdint>

namespace logger{
    constexpr uint32_t kEcsEventHex     = 0x33cccc;
    constexpr uint32_t kEcsSystemHex    = 0xee51b1;
    constexpr uint32_t kEcsComponentHex = 0xe3c515;
    constexpr uint32_t kLogTimeHex      = 0x4b2d9f;
    constexpr uint32_t kWarningHex      = 0x6600ff;
    constexpr uint32_t kErrorHex        = 0xc41e3a;
};

#endif // LOGGER_COLORS_H
