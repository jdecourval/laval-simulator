#ifndef SIMULATOR_OPCODES_H
#define SIMULATOR_OPCODES_H

#include <array>
#include <cassert>
#include <functional>
#include <memory>
#include <fstream>
#include <typeinfo>

#include "registers.h"



struct InstructionBase
{
    virtual void operator()(Registers& registers) const = 0;
};


// TODO: Variadic arguments
template<uint8_t... ArgSize>
struct Instruction : public InstructionBase
{
    using Args = std::index_sequence<ArgSize...>;


    Instruction()
    {
        static_assert(sizeof...(ArgSize) == 0, "Not Default Constructible");
    }

    explicit Instruction(uint8_t)
    {
        // TODO: Initialize args
    }

    uint8_t argument() const;


protected:
	static void sync(Registers& registers);

    std::array<uint8_t, sizeof...(ArgSize)> args;
};


// 1 bits instructions

// Do nothing for one cycle
// arg: none
struct NOP : public Instruction<>
{
//    using Instruction<>::Instruction;

    void operator()(Registers& registers) const override;
};

// Sync current VAL with connected mux
// arg: none
struct SYN : public Instruction<>
{
	using Instruction<>::Instruction;

	void operator()(Registers& registers) const override;
};

// Connect to carry
// arg: none
struct CTC : public Instruction<>
{
	using Instruction<>::Instruction;

	void operator()(Registers& registers) const override;
};

// Connect to val
// arg: none
struct CTV : public Instruction<>
{
	using Instruction<>::Instruction;

	void operator()(Registers& registers) const override;
};

// Output some debug values to debugger
// arg: none
struct DBG : public Instruction<>
{
	using Instruction<>::Instruction;

	void operator()(Registers& registers) const override;
};

// Should have a really weird but maybe useful behaviour. Setup phase ? Switch to 16 bits? Use mux as another register?
// arg: none
struct HCF : public Instruction<>
{
	using Instruction<>::Instruction;

	void operator()(Registers& registers) const override;
};


// TODO ?: Change sync mode: auto, manual

// 2 bits instructions
// Load from mux
// Arg 1:1: Sync after
struct MXL : public Instruction<1>
{
	using Instruction<1>::Instruction;

	void operator()(Registers& registers) const override;
};

// Add from mux
// Arg 1:1: Sync after
struct MXA : public Instruction<1>
{
	using Instruction<1>::Instruction;

	void operator()(Registers& registers) const override;
};

// Subtract from mux
// Arg 1:1: Sync after
struct MXS : public Instruction<1>
{
	using Instruction<1>::Instruction;

	void operator()(Registers& registers) const override;
};


// 4 bits instructions

// Change mux address
// arg 1:3: address
struct MUX : public Instruction<3>
{
	using Instruction<3>::Instruction;

	void operator()(Registers& registers) const override;
};

//// Change mux address
//// arg 1:4: address
//struct HMX : public Instruction<4>
//{
//	using Instruction<4>::Instruction;
//
//	void operator()(Registers& registers) const override;
//};

// Load constant into low part
// arg 1:4: constant
struct LCL : public Instruction<4>
{
	using Instruction<4>::Instruction;

	void operator()(Registers& registers) const override;
};

// Load constant into high part
// arg 1:4: constant
struct LCH : public Instruction<4>
{
	using Instruction<4>::Instruction;

	void operator()(Registers& registers) const override;
};

// Jump if value less than 0. Change memory bank
// arg 1:4: memory bank address
struct JLV : public Instruction<4>
{
	using Instruction<4>::Instruction;

	void operator()(Registers& registers) const override;
};

// Jump if value equals 0
// arg 1:4: memory bank address
struct JEV : public Instruction<4>
{
	using Instruction<4>::Instruction;

	void operator()(Registers& registers) const override;
};

// Jump if value greater than 0
// arg 1:4: memory bank address
struct JGV : public Instruction<4>
{
	using Instruction<4>::Instruction;

	void operator()(Registers& registers) const override;
};

// Jump unconditionally
// arg 1:4: memory bank address
struct JMP : public Instruction<4>
{
	using Instruction<4>::Instruction;

	void operator()(Registers& registers) const override;
};


// 5 bits instructions

// Left circular shift
// arg 1:4: shift amount
// Arg 2:1: Sync after
struct LSH : public Instruction<5>
{
	using Instruction<5>::Instruction;

	void operator()(Registers& registers) const override;
};

// Right circular shift
// arg 1:4: shift amount
// Arg 2:1: Sync after
struct RSH : public Instruction<5>
{
	using Instruction<5>::Instruction;

	void operator()(Registers& registers) const override;
};

// Add a constant
// arg 1:4: constant
// Arg 2:1: Sync after
struct CAD : public Instruction<5>
{
	using Instruction<5>::Instruction;

	void operator()(Registers& registers) const override;
};

// Subtract a constant
// arg 1:4: constant
// Arg 2:1: Sync after
struct CSU : public Instruction<5>
{
	using Instruction<5>::Instruction;

	void operator()(Registers& registers) const override;
};

class InstructionFactory
{
public:
    InstructionFactory()
    : dump("dump")
    {
        register_instruction<NOP>();
        register_instruction<SYN>();
        register_instruction<CTC>();
        register_instruction<CTV>();
        register_instruction<DBG>();
        register_instruction<HCF>();
        register_instruction<MXL>();
        register_instruction<MXA>();
        register_instruction<MXS>();
        register_instruction<MUX>();
        register_instruction<LCL>();
        register_instruction<LCH>();
        register_instruction<JLV>();
        register_instruction<JEV>();
        register_instruction<JGV>();
        register_instruction<JMP>();
        register_instruction<LSH>();
        register_instruction<RSH>();
        register_instruction<CAD>();
        register_instruction<CSU>();
    }

	template<typename T>
	void register_instruction()
	{
        register_helper<T>(typename T::Args{});
	}

    std::unique_ptr<InstructionBase> create(uint8_t val)
	{
        // Check if the instruction is valid
        assert(val < counter);

		return map.at(val)(val);
	}

private:
    template <typename T, size_t... I>
    void register_helper(std::index_sequence<I...>)
    {
        auto end = counter + 1L;

        if constexpr (sizeof...(I) > 0)
        {
            end = ((1 << I) + ...) + counter;
        }

        assert(end <= 256);

        auto begin = counter;

        for (; counter<end; counter++)
        {
            map.at(counter) = [begin](uint8_t val) {
                if constexpr (sizeof...(I) > 0)
                {
                    assert(val > begin);
                    return std::make_unique<T>(val - begin);
                } else if (sizeof...(I) == 0)
                {
                    assert(val == begin);
                    return std::make_unique<T>();
                }
            };

            dump << static_cast<int>(counter) << ":" << typeid(T).name() << std::endl;
        }
    }

private:
	std::array<std::function<std::unique_ptr<InstructionBase>(uint8_t)>, 256> map;
	uint8_t counter = 0;
    std::ofstream dump;

};

#include "opcodes.hpp"
#endif //SIMULATOR_OPCODES_H
