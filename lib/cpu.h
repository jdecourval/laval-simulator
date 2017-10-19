#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H

#include <memory>

#include "memory.h"
#include "impl/core_array.h"


class CoreArray;

/// Class responsible of initializing and running the system
class Cpu
{
public:
    struct Settings
    {
        constexpr static std::initializer_list<size_t> dimensions = {10, 10, 10};
        constexpr static auto bank_number = 16u;
        constexpr static auto bank_size  = 256u;
    };

    explicit Cpu(const Settings&);

    void Start();

    Cpu(Cpu&) = delete;

    Cpu& operator=(Cpu&) = delete;

private:
    void linkCores();

    bool running;
    Memory mem;
    CoreArray cores;
};


#endif //SIMULATOR_CPU_H
