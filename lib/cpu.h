#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H

#include "memory.h"
#include "core_array.h"


/// Class responsible of initializing and running the system
class Cpu
{
public:
    struct Settings
    {
        constexpr static std::initializer_list<size_t> dimensions = {10, 10, 10};
        constexpr static auto bank_number = 16;
        constexpr static auto bank_size  = 256;
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
