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
        BEFORE = -1,
        CURRENT = 0,
        AFTER = 1
    };
}

class Direction
{
public:
    explicit Direction(uint8_t raw);

    uint8_t dump();

    constexpr decltype(auto) cbegin() const noexcept;

    constexpr decltype(auto) begin() noexcept;

    constexpr decltype(auto) cend() const noexcept;

    constexpr decltype(auto) end() noexcept;

    static constexpr size_t total();

private:
    // raw value is ordered LSB to MSB
    std::array<detail::Direction1D, Settings::DIMENSION_N> directions;
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


#include "direction.hpp"

#endif //SIMULATOR_DIRECTION_H
