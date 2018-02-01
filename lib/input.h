#ifndef PROJECT_INPUT_H
#define PROJECT_INPUT_H

#include "fetchable.h"

#include <cstdint>
#include <mutex>
#include <queue>


class Input : public Fetchable
{
public:
    explicit Input(std::mutex& mutex)
    : mutex(mutex)
    {

    }

    void put(uint8_t value)
    {
        queue.push(value);
    }

    std::optional<std::pair<bool, uint8_t>> get_from(bool carry) override
    {
        cpu_assert(!carry, "Cannot get carry from input");

        std::lock_guard lock(mutex);

        if (!queue.empty())
        {
            auto value = queue.back();
            queue.pop();
            return {{false, value}};
        }
        else
        {
            return {};
        }
    }

private:
    std::queue<uint8_t> queue;
    std::mutex& mutex;
};


#endif //PROJECT_INPUT_H
