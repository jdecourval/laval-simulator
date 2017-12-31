#include "direction.h"


std::variant<Direction::CoreDirection, Direction::SpecialDirection> Direction::decode(uint8_t raw)
{
    if (raw < total_core_directions())
    {
        CoreDirection core_direction;

        for (auto& direction : core_direction)
        {
            // 3 corresponds to the number of possible Direction1D
            direction = static_cast<Direction1D>(raw % 3);
            raw /= 3;
        }

        return DirectionComplex(std::move(core_direction));
    }
    else if (raw < static_cast<uint8_t>(Direction::SpecialDirection::LAST_DO_NOT_USE))
    {
        return DirectionComplex(static_cast<SpecialDirection>(raw));
    }
    else
    {
        throw std::out_of_range{"Out of range"};
    }
}
