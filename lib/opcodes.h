#ifndef SIMULATOR_OPCODES_H
#define SIMULATOR_OPCODES_H

#include "instruction.h"
#include "registers.h"


namespace OpCodes
{

// 1 bits instructions

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


// TODO ?: Change sync mode: auto, manual

// 2 bits instructions
// Load from mux
// Arg 1:1: Sync after
    struct MXL : public Instruction<1>
    {
        using Instruction<1>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Add from mux
// Arg 1:1: Sync after
    struct MXA : public Instruction<1>
    {
        using Instruction<1>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Subtract from mux
// Arg 1:1: Sync after
    struct MXS : public Instruction<1>
    {
        using Instruction<1>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// 4 bits instructions

// Change mux address
// arg 1:5: address
    struct MUX : public Instruction<5>
    {
        using Instruction<5>::Instruction;

        bool operator()(Registers& registers) const override;
    };

// Load constant into low part
// arg 1:4: constant
    struct LCL : public Instruction<4>
    {
        using Instruction<4>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Load constant into high part
// arg 1:4: constant
    struct LCH : public Instruction<4>
    {
        using Instruction<4>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Jump if value less than 0. Change memory bank
// arg 1:4: memory bank address
    struct JLZ : public Instruction<4>
    {
        using Instruction<4>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Jump if value equals 0
// arg 1:4: memory bank address
    struct JEZ : public Instruction<4>
    {
        using Instruction<4>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Jump if value greater than 0
// arg 1:4: memory bank address
    struct JGZ : public Instruction<4>
    {
        using Instruction<4>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Jump unconditionally
// arg 1:4: memory bank address
    struct JMP : public Instruction<4>
    {
        using Instruction<4>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// 5 bits instructions

// Left logical shift
// arg 1:4: shift amount
// Arg 2:1: Sync after
    struct LLS : public Instruction<4, 1>
    {
        using Instruction<4, 1>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Right logical shift
// arg 1:4: shift amount
// Arg 2:1: Sync after
    struct RLS : public Instruction<4, 1>
    {
        using Instruction<4, 1>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Add a constant
// arg 1:4: constant
// Arg 2:1: Sync after
    struct CAD : public Instruction<4, 1>
    {
        using Instruction<4, 1>::Instruction;

        bool operator()(Registers& registers) const override;
    };


// Subtract a constant
// arg 1:4: constant
// Arg 2:1: Sync after
    struct CSU : public Instruction<4, 1>
    {
        using Instruction<4, 1>::Instruction;

        bool operator()(Registers& registers) const override;
    };


    // Logical AND
    // arg 1:4: constant
    // Arg 2:1: Sync after
    struct CAN : public Instruction<4, 1>
    {
        using Instruction<4, 1>::Instruction;

        bool operator()(Registers& registers) const override;
    };


    // Logical AND
    // arg 1:4: constant
    // Arg 2:1: Sync after
    struct COR : public Instruction<4, 1>
    {
        using Instruction<4, 1>::Instruction;

        bool operator()(Registers& registers) const override;
    };

    }

// TODO: Load a constant 0 instruction? Then maybe replace LCL and LCH by Load 0 + CAD

#endif //SIMULATOR_OPCODES_H
