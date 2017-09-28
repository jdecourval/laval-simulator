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

    Instruction(std::initializer_list<uint8_t> args);

    // TODO: This function wrongly allows to accidentally pass a single argument to a multiple arguments instruction
    explicit Instruction(uint8_t args_raw);

    uint8_t get_argument(uint8_t arg_index) const;


protected:
    static void sync(Registers& registers);

    template<typename T>
    static constexpr T bitmask(unsigned int onecount);

    template<typename T>
    static constexpr bool checkcarry(T val);


private:
    template<uint8_t LastArgSize>
    void set_arg(uint8_t args_raw, uint8_t i);

    template<uint8_t FirstArgSize, uint8_t SecondArgSize, uint8_t... OtherArgSizes>
    void set_arg(uint8_t args_raw, uint8_t i);

    void set_arg(uint8_t args_raw);


private:
    std::array<uint8_t, sizeof...(ArgSizes)> args;
};


#include "instruction.hpp"

#endif //PROJECT_INSTRUCTION_H
