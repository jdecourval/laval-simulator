#include "direction.h"


Direction::Direction(uint8_t raw)
: directions()
{
    for (auto& direction : directions)
    {
        direction = static_cast<detail::Direction1D>(raw % 3 - 1);
        raw /= 3;
    }
}

uint8_t Direction::dump()
{
    uint8_t value = 0;

    for (auto i = 0u; i < directions.size(); i++)
    {
        value += (directions.at(i) + 1) * std::pow(3, i);
    }

    return value;
}

constexpr size_t Direction::total()
{
    // 3 come from the number of possible direction: CURRENT, BEFORE, AFTER
    return static_cast<size_t>(std::pow(3, Settings::DIMENSION_N));
}

DirectionComplex::DirectionComplex(uint8_t raw)
        : std::variant<Direction, SpecialDirection>(Direction(raw))
{
    if (raw >= Direction::total())
    {
        emplace<SpecialDirection>(static_cast<SpecialDirection>(raw - Settings::CORES));
    }
}

uint8_t DirectionComplex::dump()
{
    if (std::holds_alternative<Direction>(*this))
    {
        return std::get<Direction>(*this).dump();
    }
    else
    {
        return static_cast<uint8_t>(std::get<SpecialDirection>(*this)) + Settings::CORES;
    }
}
