#ifndef SIMULATOR_OPCODES_H
#define SIMULATOR_OPCODES_H

#include "instruction.h"
#include "registers.h"


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

    explicit DBG(Registers* output);

    bool operator()(Registers& registers) const override;

private:
    Registers* output = nullptr;
};


// Should have a really weird but maybe useful behaviour. Setup phase ? Switch to 16 bits? Use mux as another register?
// arg: none
struct HCF : public Instruction<>
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
// arg 1:3: address
struct MUX : public Instruction<3>
{
    using Instruction<3>::Instruction;

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
struct JLV : public Instruction<4>
{
    using Instruction<4>::Instruction;

    bool operator()(Registers& registers) const override;
};


// Jump if value equals 0
// arg 1:4: memory bank address
struct JEV : public Instruction<4>
{
    using Instruction<4>::Instruction;

    bool operator()(Registers& registers) const override;
};


// Jump if value greater than 0
// arg 1:4: memory bank address
struct JGV : public Instruction<4>
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

// Left circular shift
// arg 1:4: shift amount
// Arg 2:1: Sync after
struct LSH : public Instruction<4, 1>
{
    using Instruction<4, 1>::Instruction;

    bool operator()(Registers& registers) const override;
};


// Right circular shift
// arg 1:4: shift amount
// Arg 2:1: Sync after
struct RSH : public Instruction<4, 1>
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


#endif //SIMULATOR_OPCODES_H
