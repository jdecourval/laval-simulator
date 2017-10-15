#include "instruction_factory.h"


InstructionFactory::InstructionFactory()
        : dump_file("dump")
{
}

std::unique_ptr<InstructionBase> InstructionFactory::create(uint8_t val)
{
    // Check if the instruction is valid
    assert(val < counter);

    return map.at(val)(val);
}

