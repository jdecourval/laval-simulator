#include "core.h"

#include <iostream>
#include <cmath>
#include <cassert>

#include "multidimensional_array.h"
#include "opcodes.h"

inline uint8_t operator "" _8(unsigned long long value)
{
    return static_cast<uint8_t>(value);
}

Core::Core()
        : mem()
          , id(0)
          , cores(nullptr)
{
//    for(size_t i=0; i <= sizeof...(RestD); i++)
//    {
//        status1.mux += std::pow(1 * 3, i);
//    }

//    mem.fill(Instruction{Op::NOP, 0});
}


void Core::check()
{

}

void Core::link(CoreArray *cores, size_t id, Memory_t *mem)
{
    this->cores = cores;
    this->id = id;
    this->mem = mem;

}

void Core::step1()
{
    auto direction_complex = DirectionComplex(registers.status1.mux);

    try
    {
        auto direction = std::get<Direction>(direction_complex);

        auto& pointed_core = cores->offset(id, direction);

        if (pointed_core.registers.status1.sync)
        {
            registers.preload = registers.status1.ctc ? pointed_core.registers.status2.carry : pointed_core.registers.val;
        }
        else
        {
            registers.preload = {};
        }
    } catch(const std::bad_variant_access&)
    {
        auto direction = std::get<SpecialDirection>(direction_complex);

        switch (direction)
        {
            case SpecialDirection::PC:
                registers.preload = registers.pc;
                break;

            case SpecialDirection::MEMBANK:
                registers.preload = registers.status2.membank;
                break;

            default:
                throw 1;
        }
    }
}


void Core::step2()
{
    if (!registers.preload)
    {
        return;
    }

    registers.status1.sync = false;

    auto raw_instruction = (*mem)[registers.status2.membank][registers.pc & 0xf];
    // Let it wrap around
    registers.pc++;
//    Op op = (*mem)[registers.status2.membank][registers.pc & 0xf].opcode;
//    uint8_t arg = (*mem)[registers.status2.membank][registers.pc & 0xf].arg;

    auto instruction = factory.create(raw_instruction);
    (*instruction)(registers);
}


void Core::sync()
{
    registers.status1.sync = true;
}

void Core::execute(const InstructionBase& raw_instruction)
{
    raw_instruction(registers);
}


