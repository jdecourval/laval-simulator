#ifndef SIMULATOR_CORE_H
#define SIMULATOR_CORE_H

#include "instruction_factory.h"
#include "registers.h"
#include "memory.h"


class CoreArray;


class Core
{
public:
    Core();

    Core(const CoreArray& cores, size_t id, const Memory_t& mem);

    void preload();

    void fetch();

    bool execute(const InstructionBase& raw_instruction);

    void sync();

    bool operator==(const Core&) const;

    Core(Core&& core) = default;

    Core(Core&) = delete;

    Core& operator=(Core&) = delete;

private:
    Registers registers;
    const Memory_t* mem;

    const CoreArray* cores;
    InstructionFactory factory;

    friend std::ostream& operator<<(std::ostream& os, const Core& core);
};


std::ostream& operator<<(std::ostream& os, const Core& core);


#include "core.hpp"

#endif //SIMULATOR_CORE_H
