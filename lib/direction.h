#ifndef SIMULATOR_DIRECTION_H
#define SIMULATOR_DIRECTION_H

#include <cstdint>
#include <cmath>
#include <tuple>
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

// TODO: Should instead store an array
class Direction : public std::array<detail::Direction1D, Settings::DIMENSION_N>
{
public:
    explicit Direction(uint8_t raw);

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
    explicit DirectionComplex(uint8_t raw);

    uint8_t dump();
};


#endif //SIMULATOR_DIRECTION_H
