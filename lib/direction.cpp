#include "direction.h"


Direction::Direction()
: Direction(std::array<Direction1D, Dimensions>{})
{

}

Direction::Direction(uint8_t raw)
    : directions()
{
#ifndef NDEBUG
    if (raw >= total())
    {
        throw std::out_of_range{"Out of range"};
    }
#endif

    for (auto& direction : directions)
    {
        // 3 corresponds to the number of possible Direction1D
        direction = static_cast<Direction1D>(raw % 3 - 1);
        raw /= 3;
    }
}

Direction::Direction(const std::array<Direction1D, Dimensions>& directions)
: directions(directions)
{

}

uint8_t Direction::dump()
{
    unsigned value = 0;

    for (auto i = 0u; i < directions.size(); i++)
    {
        // 3 corresponds to the number of possible Direction1D
        // TODO: Check for overflow
        value += (directions.at(i) + 1u) * three_pow(i);
    }

    assert(value <= std::numeric_limits<uint8_t>::max());
    return static_cast<uint8_t>(value);
}

DirectionComplex::DirectionComplex(uint8_t raw)
    : Variant(raw >= Direction::total()
              ? Variant(static_cast<SpecialDirection>(raw - Direction::total()))
              : Variant(Direction(raw))
)
{
#ifndef NDEBUG
    if (raw >= Direction::total() + static_cast<int>(SpecialDirection::LAST_DO_NOT_USE))
    {
        throw std::out_of_range{"Out of range"};
    }
#endif
}

uint8_t DirectionComplex::dump()
{
    if (std::holds_alternative<Direction>(*this))
    {
        return std::get<Direction>(*this).dump();
    }
    else
    {
        return static_cast<uint8_t>(std::get<SpecialDirection>(*this)) + Direction::total();
    }
}

DirectionComplex::DirectionComplex(const Direction& direction)
    : Variant(direction)
{

}

DirectionComplex::DirectionComplex(SpecialDirection direction)
    : Variant(direction)
{
#ifndef NDEBUG
    if (static_cast<int>(direction) >= static_cast<int>(SpecialDirection::LAST_DO_NOT_USE))
    {
        throw std::out_of_range{"Out of range"};
    }
#endif
}
