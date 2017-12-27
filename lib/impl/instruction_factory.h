#ifndef PROJECT_INSTRUCTION_FACTORY_H
#define PROJECT_INSTRUCTION_FACTORY_H

#include "instruction.h"

#include <fstream>
#include <functional>
#include <memory>


// TODO: Static class or members ? Currently there is one instance per core
class InstructionFactory
{
public:
    template<typename T>
    void register_instruction();

    std::unique_ptr<InstructionBase> create(uint8_t val);

    std::unique_ptr<InstructionBase> create(const std::pair<std::string, std::vector<uint8_t>>& ast);

    uint8_t dump(const InstructionBase& instruction) const;

private:
    template<typename T, size_t... I>
    void register_helper(std::index_sequence<I...>);

private:
    std::array<std::function<std::unique_ptr<InstructionBase>(uint8_t)>, 256> map;
    std::unordered_map<size_t, uint8_t> instruction_to_offset;
    std::unordered_map<std::string, size_t> name_to_instruction;
    uint8_t counter = 0;
};


#include "instruction_factory.hpp"

#endif //PROJECT_INSTRUCTION_FACTORY_H
