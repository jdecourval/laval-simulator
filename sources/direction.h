#ifndef SIMULATOR_DIRECTION_H
#define SIMULATOR_DIRECTION_H

#include <cstdint>
#include <tuple>
#include <cmath>
#include <variant>

#include "settings.h"

namespace detail
{
    enum Direction1D
    {
        CURRENT = -1,
        BEFORE = 0,
        AFTER = 1
    };
}

class Direction : public std::array<detail::Direction1D, Settings::DIMENSION_N>
{
public:
    Direction(uint8_t raw);

    uint8_t dump();
};

enum class SpecialDirection
{
    PC,
    MEMBANK
};

class DirectionComplex : public std::variant<Direction, SpecialDirection>
{
public:
    DirectionComplex(uint8_t raw);

    uint8_t dump();
};

#endif //SIMULATOR_DIRECTION_H
