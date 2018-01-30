#ifndef PROJECT_FETCHABLE_H
#define PROJECT_FETCHABLE_H

#include <cstdint>
#include <utility>
#include <optional>


class Fetchable
{
public:
    // TODO: Replace pair by struct
    virtual std::optional<std::pair<bool, uint8_t>> get_from(bool carry) = 0;
};

#endif //PROJECT_FETCHABLE_H
