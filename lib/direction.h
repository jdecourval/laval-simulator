#ifndef SIMULATOR_DIRECTION_H
#define SIMULATOR_DIRECTION_H

#include <cstdint>
#include <cmath>
#include <tuple>
#include <variant>


class DirectionComplex;

class Direction
{
    static constexpr auto Dimensions = 3u;

public:
    enum Direction1D
    {
        BEFORE = -1,
        CURRENT = 0,
        AFTER = 1
    };

    Direction();

    explicit Direction(uint8_t raw);

    explicit Direction(const std::array<Direction1D, Dimensions>& directions);

    uint8_t dump();

    constexpr decltype(auto) cbegin() const noexcept;

    constexpr decltype(auto) begin() noexcept;

    constexpr decltype(auto) cend() const noexcept;

    constexpr decltype(auto) end() noexcept;

    static constexpr size_t size();

    static constexpr size_t total();

private:
    static constexpr uint8_t three_pow(unsigned long n);

    // raw value is ordered LSB to MSB
    std::array<Direction1D, Dimensions> directions;
};


enum class SpecialDirection: uint8_t
{
    PC,
    MEMBANK,
    LAST_DO_NOT_USE
};


class DirectionComplex : public std::variant<Direction, SpecialDirection>
{
    using Variant = std::variant<Direction, SpecialDirection>;

public:
    explicit DirectionComplex(SpecialDirection);
    explicit DirectionComplex(const Direction&);
    explicit DirectionComplex(uint8_t raw);

    uint8_t dump();
};


#include "impl/direction.hpp"

#endif //SIMULATOR_DIRECTION_H
