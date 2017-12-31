#ifndef SIMULATOR_DIRECTION_H
#define SIMULATOR_DIRECTION_H

#include "impl/tools.h"

#include <variant>


namespace Direction
{
    enum Direction1D
    {
        BEFORE = 0,
        CURRENT = 1,
        AFTER = 2
    };

    using CoreDirection = std::array<Direction1D, 3>;

    constexpr unsigned total_core_directions()
    {
        return Tools::three_pow(Direction::AFTER + 1);
    }

    enum class SpecialDirection: uint8_t
    {
        PC = total_core_directions(),
        MEMBANK,
        LAST_DO_NOT_USE
    };

    using DirectionComplex = std::variant<CoreDirection, SpecialDirection>;

    DirectionComplex decode(uint8_t);
}

#endif //SIMULATOR_DIRECTION_H
