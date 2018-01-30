#ifndef PROJECT_INPUT_H
#define PROJECT_INPUT_H

#include "fetchable.h"

#include <cstdint>
#include <optional>


class Input : public Fetchable
{
public:
    void put(uint8_t value)
    {
        this->value = value;
    }

    std::optional<std::pair<bool, uint8_t>> get_from(bool carry) override
    {
        cpu_assert(!carry, "Cannot get carry from input");

        return {{false, 8}};
        if (value)
        {
            return {{false, *value}};
        }
        else
        {
            return {};
        }
    }

private:
    std::optional<uint8_t> value;
};


#endif //PROJECT_INPUT_H
