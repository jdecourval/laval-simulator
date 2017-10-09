#include <iostream>
#include "core.h"

#include "core_array.h"


Core::Core()
        : registers()
          , mem()
          , cores(nullptr)
{
    // This simulator rely on the fact that the simulating CPU uses two's complement notation
    static_assert(-1 == ~0, "Your CPU architecture is too weird to use this simulator");
}

Core::Core(const CoreArray& cores, size_t id, const Memory_t& mem)
    : registers()
      , mem(&mem)
      , cores(&cores)
{
    // This simulator rely on the fact that the simulating CPU uses two's complement notation
    static_assert(-1 == ~0, "Your CPU architecture is too weird to use this simulator");

    registers.id = id;
}


void Core::preload()
{
    auto direction_complex = DirectionComplex(registers.status1.mux);

    try
    {
        assert(cores);

        auto direction = std::get<Direction>(direction_complex);

        auto& pointed_core = cores->offset(registers.id, direction);

        if (pointed_core.registers.status1.sync)
        {
            registers.preload = registers.status1.ctc ? pointed_core.registers.status2.carry
                                                      : pointed_core.registers.val;
            registers.preload_negative = pointed_core.registers.status2.negative;
        }
        else
        {
            registers.preload = {};
        }
    }
    catch (const std::bad_variant_access&)
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

            case SpecialDirection::VAL:
                registers.preload = registers.val;
                break;

            default:
                throw std::logic_error("Invalid direction");
        }
    }
}

void Core::fetch()
{
#ifndef NDEBUG
    if (!mem)
    {
        throw std::logic_error{"Memory is not linked"};
    }
#endif

    registers.status1.sync = false;

    auto raw_instruction = (*mem)[registers.status2.membank][registers.pc & 0xf];

    auto instruction = factory.create(raw_instruction);

    // Increment PC only if the instruction does not needs to stall the pipeline
    if ((*instruction)(registers))
    {
        // Let it wrap around
        registers.pc++;
    }
}

void Core::sync()
{
    registers.status1.sync = true;
}

bool Core::execute(const InstructionBase& raw_instruction)
{
    return raw_instruction(registers);
}

bool Core::operator==(const Core& core) const
{
    return registers.id == core.registers.id;
}

std::ostream& operator<<(std::ostream& os, const Core& core)
{
    os << core.registers.id;
    return os;
}
