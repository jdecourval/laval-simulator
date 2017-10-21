#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H

#include <memory>
#include <chrono>

#include "memory.h"
#include "impl/core_array.h"


class CoreArray;

/// Class responsible of initializing and running the system
class Cpu
{
    using BenchmarkClock = std::chrono::high_resolution_clock;

public:
    struct Settings
    {
        constexpr static std::initializer_list<size_t> dimensions = {10, 10, 10};
        constexpr static auto bank_number = 16u;
        constexpr static auto bank_size  = 256u;
    };

    explicit Cpu(const Settings&);
    explicit Cpu(const Settings&, Memory&& memory);

    [[noreturn]] void Start(const std::chrono::milliseconds& period);

    Cpu(Cpu&) = delete;

    Cpu& operator=(Cpu&) = delete;

private:
    Memory mem;
    CoreArray cores;
};


#endif //SIMULATOR_CPU_H
