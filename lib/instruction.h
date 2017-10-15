#ifndef PROJECT_INSTRUCTION_H
#define PROJECT_INSTRUCTION_H

#include <cassert>
#include <cstddef>

#include "registers.h"


struct InstructionBase
{
    /// @return false if the instruction have to stall the pipeline
    virtual bool operator()(Registers& registers) const = 0;
};


template<uint8_t... ArgSizes>
struct Instruction : public InstructionBase
{
    using Args = std::index_sequence<ArgSizes...>;

    constexpr Instruction();

    constexpr Instruction(std::array<uint8_t, sizeof...(ArgSizes)> args);

    constexpr explicit Instruction(std::byte args_raw);

    uint8_t dump() const;


protected:
    constexpr uint8_t get_argument(uint8_t arg_index) const;

    static constexpr void sync(Registers& registers);

    template<typename T>
    static constexpr T bitmask(unsigned int onecount);

    template<typename T>
    static constexpr bool checkcarry(T val);


private:
    template<uint8_t LastArgSize>
    constexpr void set_arg(uint8_t args_raw, uint8_t i);

    template<uint8_t FirstArgSize, uint8_t SecondArgSize, uint8_t... OtherArgSizes>
    constexpr void set_arg(uint8_t args_raw, uint8_t i);

    constexpr void set_arg(uint8_t args_raw);

    template<uint8_t LastArgSize>
    uint8_t dump(uint8_t i, uint8_t shift) const;

    template<uint8_t FirstArgSize, uint8_t SecondArgSize, uint8_t... OtherArgSizes>
    uint8_t dump(uint8_t i, uint8_t shift) const;


private:
    std::array<uint8_t, sizeof...(ArgSizes)> args;
};


#include "instruction.hpp"

#endif //PROJECT_INSTRUCTION_H
