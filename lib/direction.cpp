#include "direction.h"

Direction::Direction(uint8_t raw)
{
    for (auto i = 0; i < size(); i++)
    {
        this->at(i) = static_cast<detail::Direction1D>(raw % 3 - 1);
        raw /= 3;
    }
}

uint8_t Direction::dump()
{
    uint8_t value = 0;

    for (auto i = 0; i < size(); i++)
    {
        value += (this->at(i) + 1) * std::pow(i + 1, 3);
    }

    return value;
}

DirectionComplex::DirectionComplex(uint8_t raw)
: std::variant<Direction, SpecialDirection>(Direction(raw))
{
    if (raw >= Settings::CORES)
    {
        std::variant<Direction, SpecialDirection>(SpecialDirection(raw - Settings::CORES));
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
