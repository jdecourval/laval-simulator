#ifndef PROJECT_INSTRUCTION_FACTORY_H
#define PROJECT_INSTRUCTION_FACTORY_H

#include <fstream>
#include <functional>
#include <memory>

#include "instruction.h"


// TODO: Static class or members ?
class InstructionFactory
{
public:
    InstructionFactory();

    template<typename T>
    void register_instruction();

    std::unique_ptr<InstructionBase> create(uint8_t val);

private:
    template<typename T, size_t... I>
    void register_helper(std::index_sequence<I...>);

private:
    std::array<std::function<std::unique_ptr<InstructionBase>(uint8_t)>, 256> map;
    uint8_t counter = 0;
    std::ofstream dump;

};


#include "instruction_factory.hpp"

#endif //PROJECT_INSTRUCTION_FACTORY_H
