#ifndef PROJECT_INSTRUCTION_H
#define PROJECT_INSTRUCTION_H

#include "registers.h"

#include <cassert>
#include <cstddef>
#include <vector>


struct InstructionBase
{
    /// @return false if the instruction have to stall the pipeline
    virtual bool operator()(Registers& registers) const = 0;

    virtual uint8_t dump_args() const = 0;

    virtual void load_args(const std::vector<uint8_t>& args) = 0;
};


template<uint8_t... ArgsMaxes>
struct Instruction : public InstructionBase
{
    using Args = std::index_sequence<ArgsMaxes...>;

    constexpr Instruction();

    constexpr Instruction(const std::array<uint8_t, sizeof...(ArgsMaxes)>& args);

    constexpr explicit Instruction(std::byte args_raw);

    uint8_t dump_args() const override final;

    void load_args(const std::vector<uint8_t>& args) override;


protected:
    constexpr uint8_t get_argument(uint8_t arg_index) const;

    static constexpr void sync(Registers& registers);

    template<typename T>
    static constexpr bool checkcarry(T val);


private:
    template<uint8_t LastArgSize>
    constexpr void set_arg(uint8_t args_raw, uint8_t i);

    template<uint8_t FirstArgSize, uint8_t SecondArgSize, uint8_t... OtherArgsMaxes>
    constexpr void set_arg(uint8_t args_raw, uint8_t i);

    constexpr void set_arg(uint8_t args_raw);

    template<uint8_t LastArgSize>
    uint8_t dump_args(uint8_t i, uint8_t shift) const;

    template<uint8_t FirstArgSize, uint8_t SecondArgSize, uint8_t... OtherArgsMaxes>
    uint8_t dump_args(uint8_t i, uint8_t shift) const;


private:
    // Arguments are packed in little-endianish notation (last argument have the smallest numerical value)
    std::array<uint8_t, sizeof...(ArgsMaxes)> args;
};


#include "impl/instruction.hpp"

#endif //PROJECT_INSTRUCTION_H
