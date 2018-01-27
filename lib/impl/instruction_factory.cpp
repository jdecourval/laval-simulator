#include "instruction_factory.h"


std::unique_ptr<InstructionBase> InstructionFactory::create(uint8_t val)
{
    // Check if the instruction is valid
    cpu_assert(val < counter, "Invalid instruction");

    return map.at(val)(val);
}

std::unique_ptr<InstructionBase> InstructionFactory::create(const std::pair<std::string, std::vector<uint8_t>>& ast)
{
    cpu_assert(name_to_instruction.count(ast.first), "Unknown instruction")

    auto instruction = create(name_to_instruction.at(ast.first));

    instruction->load_args(ast.second);

    return instruction;
}

uint8_t InstructionFactory::dump(const InstructionBase& instruction) const
{
    return instruction_to_offset.at(typeid(instruction).hash_code()) + instruction.dump_args();
}

uint8_t InstructionFactory::size() const
{
    return counter;
}
