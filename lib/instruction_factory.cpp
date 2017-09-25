#include "instruction_factory.h"

#include "opcodes.h"


InstructionFactory::InstructionFactory()
        : dump("dump")
{
    // TODO: Move somewhere else? In core?
    register_instruction<NOP>();
    register_instruction<SYN>();
    register_instruction<CTC>();
    register_instruction<CTV>();
    register_instruction<DBG>();
    register_instruction<HCF>();
    register_instruction<MXL>();
    register_instruction<MXA>();
    register_instruction<MXS>();
    register_instruction<MUX>();
    register_instruction<LCL>();
    register_instruction<LCH>();
    register_instruction<JLV>();
    register_instruction<JEV>();
    register_instruction<JGV>();
    register_instruction<JMP>();
    register_instruction<LSH>();
    register_instruction<RSH>();
    register_instruction<CAD>();
    register_instruction<CSU>();
}

std::unique_ptr<InstructionBase> InstructionFactory::create(uint8_t val)
{
    // Check if the instruction is valid
    assert(val < counter);

    return map.at(val)(val);
}

