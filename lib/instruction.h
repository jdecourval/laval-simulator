#ifndef PROJECT_INSTRUCTION_H
#define PROJECT_INSTRUCTION_H

#include <cassert>

#include "registers.h"


struct InstructionBase
{
    virtual void operator()(Registers& registers) const = 0;
};


template<uint8_t... ArgSizes>
struct Instruction : public InstructionBase
{
    using Args = std::index_sequence<ArgSizes...>;


    Instruction();

    explicit Instruction(uint8_t args_raw);

    uint8_t get_argument(uint8_t arg_index) const;


protected:
    static void sync(Registers& registers);


private:
    template<uint8_t LastArgSize>
    void set_arg(uint8_t args_raw, uint8_t i);;

    template<uint8_t... OtherArgSizes, uint8_t LastArgSize>
    void set_arg(uint8_t args_raw, uint8_t i);;

    void set_arg(uint8_t args_raw);


private:
    std::array<uint8_t, sizeof...(ArgSizes)> args;
};


#include "instruction.hpp"

#endif //PROJECT_INSTRUCTION_H
