#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H

#include "memory.h"
#include "core_array.h"


class Cpu
{
public:
    Cpu();

    void Start();

    Cpu(Cpu&) = delete;

    Cpu& operator=(Cpu&) = delete;

private:
    void linkCores();

    bool running;
    Memory_t mem;
    CoreArray cores;
};


#endif //SIMULATOR_CPU_H
