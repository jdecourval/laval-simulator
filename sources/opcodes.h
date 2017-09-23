#ifndef SIMULATOR_OPCODES_H
#define SIMULATOR_OPCODES_H

#include <limits>
#include <bitset>


// TODO: Variadic arguments
template<uint8_t Op, uint8_t ArgSize>
struct Instruction : public std::bitset<8>
{
    Instruction(uint8_t val);

    static constexpr uint8_t mask();

    static uint8_t opcode();

    uint8_t argument();

    static bool validate(uint8_t arg);

    uint8_t operator&(uint8_t other) const;

    uint8_t operator^(uint8_t other) const;
};


// 1 bits instructions

// Do nothing for one cycle
// arg: none
using NOP = Instruction<0b1000'0000, 0>;

// Sync current VAL with connected mux
// arg: none
using SYN = Instruction<0b1000'0000, 0>;

// Connect to carry
// arg: none
using CTC = Instruction<0b1000'0000, 0>;

// Connect to val
// arg: none
using CTV = Instruction<0b1000'0000, 0>;

// Output some debug values to debugger
// arg: none
using DBG = Instruction<0b1000'0000, 0>;

// Should have a really weird but maybe useful behaviour. Setup phase ? Switch to 16 bits? Use mux as another register?
// arg: none
using HCF = Instruction<0b1000'0000, 0>;


// TODO ?: Change sync mode: auto, manual

// 2 bits instructions
// Load from mux
// Arg 1:1: Sync after
using MXL = Instruction<0b1000'0000, 1>;

// Add from mux
// Arg 1:1: Sync after
using MXA = Instruction<0b1000'0000, 1>;

// Subtract from mux
// Arg 1:1: Sync after
using MXS = Instruction<0b1000'0000, 1>;


// 4 bits instructions

// Change mux address
// arg 1:3: address
using MUX = Instruction<0b1000'0000, 3>;

//// Change mux address
//// arg 1:4: address
//using HMX = Instruction<0b1000'0000, 4>;

// Load constant into low part
// arg 1:4: constant
using LCL = Instruction<0b1000'0000, 4>;

// Load constant into high part
// arg 1:4: constant
using LCH = Instruction<0b1000'0000, 4>;

// Jump if value less than 0. Change memory bank
// arg 1:4: memory bank address
using JLV = Instruction<0b1000'0000, 4>;

// Jump if value equals 0
// arg 1:4: memory bank address
using JEV = Instruction<0b1000'0000, 4>;

// Jump if value greater than 0
// arg 1:4: memory bank address
using JGV = Instruction<0b1000'0000, 4>;

// Jump unconditionally
// arg 1:4: memory bank address
using JMP = Instruction<0b1000'0000, 4>;


// 5 bits instructions

// Left circular shift
// arg 1:4: shift amount
// Arg 2:1: Sync after
using LSH = Instruction<0b1000'0000, 5>;

// Right circular shift
// arg 1:4: shift amount
// Arg 2:1: Sync after
using RSH = Instruction<0b1000'0000, 5>;

// Add a constant
// arg 1:4: constant
// Arg 2:1: Sync after
using CAD = Instruction<0b1000'0000, 5>;

// Subtract a constant
// arg 1:4: constant
// Arg 2:1: Sync after
using CSU = Instruction<0b1000'0000, 5>;

#include "opcodes.hpp"
#endif //SIMULATOR_OPCODES_H
