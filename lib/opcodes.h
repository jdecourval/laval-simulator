#ifndef SIMULATOR_OPCODES_H
#define SIMULATOR_OPCODES_H

#include "direction.h"
#include "instruction.h"
#include "registers.h"


namespace OpCodes
{
// Do nothing for one cycle
// arg: none
    struct NOP : public Instruction<>
    {
        using Instruction<>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Sync current VAL with connected mux
// arg: none
    struct SYN : public Instruction<>
    {
        using Instruction<>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Connect to carry
// arg: none
    struct CTC : public Instruction<>
    {
        using Instruction<>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Connect to val
// arg: none
    struct CTV : public Instruction<>
    {
        using Instruction<>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Output some debug values to debugger
// arg: none
    struct DBG : public Instruction<>
    {
        using Instruction<>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Should have a really weird but maybe useful behaviour. Setup phase ? Switch to 16 bits? Use mux as another register?
// arg: none
    struct HCF : public Instruction<>
    {
        using Instruction<>::Instruction;

        bool operator()(Registers& registers) const override;
    };

// Halt the program and return current value as solution
// arg: none
    struct HLT : public Instruction<>
    {
        using Instruction<>::Instruction;

        bool operator()(Registers& registers) const override;
    };

// Fetch and discard value at mux
// arg: none
    struct MXD : public Instruction<>
    {
        using Instruction<>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Load from mux
// Arg 1:1: Sync after
    struct MXL : public Instruction<>
    {
        using Instruction<>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Add from mux
// Arg 1:1: Sync after
    struct MXA : public Instruction<>
    {
        using Instruction<>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Subtract from mux
// Arg 1:1: Sync after
    struct MXS : public Instruction<>
    {
        using Instruction<>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Make mux points to another core
// arg 1:2: offset on dimension 0
// arg 3:4: offset on dimension 1
// arg 5:6: offset on dimension 2
    struct MUX : public Instruction<2, 2, 2>
    {
        using Instruction<2, 2, 2>::Instruction;

        bool operator()(Registers& registers) const override;

//        void load_args(const std::vector<uint8_t>& args) override;
    };

// Make mux points to a register
    struct MXR : public Instruction<static_cast<uint8_t>(Direction::SpecialDirection::LAST_DO_NOT_USE)>
    {
        using Instruction<static_cast<uint8_t>(Direction::SpecialDirection::LAST_DO_NOT_USE)>::Instruction;

        bool operator()(Registers& registers) const override;
    };

// Load constant into low part
// arg 1:0xf: constant
    struct LCL : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Load constant into high part
// arg 1:0xf: constant
    struct LCH : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Jump if value less than 0. Change memory bank
// arg 1:0xf: memory bank address
    struct JLZ : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Jump if value equals 0
// arg 1:0xf: memory bank address
    struct JEZ : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Jump if value greater than 0
// arg 1:0xf: memory bank address
    struct JGZ : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Jump unconditionally
// arg 1:0xf: memory bank address
    struct JMP : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Left logical shift
// arg 1:0xf: shift amount
    struct LLS : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Right logical shift
// arg 1:0xf: shift amount
    struct RLS : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Add a constant
// arg 1:0xf: constant
    struct CAD : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Subtract a constant
// arg 1:0xf: constant
    struct CSU : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };


    // Logical AND
    // arg 1:0xf: constant
    struct CAN : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };


    // Logical OR
    // arg 1:0xf: constant
    struct COR : public Instruction<0xf>
    {
        using Instruction<0xf>::Instruction;

        bool operator()(Registers& registers) const override;
    };

    }

// TODO: Load a constant 0 instruction? Then maybe replace LCL and LCH by Load 0 + CAD

#endif //SIMULATOR_OPCODES_H
