#ifndef SIMULATOR_CPU_H
#define SIMULATOR_CPU_H

#include "memory.h"
#include "core_array.h"
#include "settings.h"


/// Class responsible of initializing and running the system
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
    Memory<UserSettings::BANK_NUMBER, UserSettings::BANK_SIZE> mem;
    CoreArray cores;
};


#endif //SIMULATOR_CPU_H
