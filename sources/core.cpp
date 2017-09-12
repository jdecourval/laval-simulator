#include "core.h"

#include <iostream>
#include <cmath>
#include <cassert>

#include "multidimensional_array.h"

inline uint8_t operator "" _8(unsigned long long value)
{
    return static_cast<uint8_t>(value);
}

Core::Core()
        : mem()
          , val(0)
          , preload(0)
          , pc(0)
          , status1()
          , status2()
          , id(0)
          , cores(nullptr)
{
//    for(size_t i=0; i <= sizeof...(RestD); i++)
//    {
//        status1.mux += std::pow(1 * 3, i);
//    }

//    mem.fill(Instruction{Op::NOP, 0});
}


void Core::check()
{
    assert(sizeof(Status1) == 1);
    assert(sizeof(Status2) == 1);
}

void Core::link(CoreArray *cores, size_t id, Memory_t *mem)
{
    this->cores = cores;
    this->id = id;
    this->mem = mem;

}

void Core::step1()
{
    auto direction_complex = DirectionComplex(status1.mux);

    try
    {
        auto direction = std::get<Direction>(direction_complex);

        auto& pointed_core = cores->offset(id, direction);

        if (pointed_core.status1.sync)
        {
            preload = status1.ctc ? pointed_core.status2.carry : pointed_core.val;
        }
        else
        {
            preload = {};
        }
    } catch(const std::bad_variant_access&)
    {
        auto direction = std::get<SpecialDirection>(direction_complex);

        switch (direction)
        {
            case SpecialDirection::PC:
                preload = pc;

            case SpecialDirection::MEMBANK:
                preload = status2.membank;

            default:
                throw 1;
        }
    }
}


void Core::step2()
{
    if (!preload)
    {
        return;
    }

    status1.sync = false;

    auto raw_instruction = (*mem)[status2.membank][pc & 0xf];
    // Let it wrap around
    pc++;
//    Op op = (*mem)[status2.membank][pc & 0xf].opcode;
//    uint8_t arg = (*mem)[status2.membank][pc & 0xf].arg;

    if (NOP::validate(raw_instruction))
    {
    }

    else if (SYN::validate(raw_instruction))
    {
        sync();
    }

    else if (CTC::validate(raw_instruction))
    {
        status1.ctc = true;
    }

    else if (CTV::validate(raw_instruction))
    {
        status1.ctc = false;
    }

    else if (DBG::validate(raw_instruction))
    {
        std::cout << "id: " << id << std::endl;
        std::cout << "pc: " << (pc & 0xf) << std::endl;
        std::cout << "membank: " << static_cast<int>(status2.membank) << std::endl;
        std::cout << "val: " << static_cast<int>(val) << std::endl;
        std::cout << "mux: " << static_cast<int>(status1.mux) << " -> " << static_cast<int>(*preload) << std::endl;
    }

    else if (HCF::validate(raw_instruction))
    {
        auto instruction = HCF(raw_instruction);
        // TODO
    }

    else if (MXL::validate(raw_instruction))
    {
        auto instruction = MXL(raw_instruction);

        val = *preload;

        if (instruction.argument() & 1)
        {
            sync();
        }
    }

    else if (MXA::validate(raw_instruction))
    {
        auto instruction = MXA(raw_instruction);

        val += *preload;

        if (instruction.argument() & 1)
        {
            sync();
        }
    }

    else if (MXS::validate(raw_instruction))
    {
        auto instruction = MXS(raw_instruction);

        val -= *preload;

        if (instruction.argument() & 1)
        {
            sync();
        }
    }

    else if (MUX::validate(raw_instruction))
    {
        auto instruction = MUX(raw_instruction);

        status1.mux = instruction.argument();
    }

    else if (LCL::validate(raw_instruction))
    {
        auto instruction = LCL(raw_instruction);
        val = static_cast<uint8_t>((val & 0xf0) | instruction.argument());
    }

    else if (LCH::validate(raw_instruction))
    {
        auto instruction = LCH(raw_instruction);
        val = static_cast<uint8_t>((instruction.argument() << 4) | (val & 0x0f));
    }

    else if (JLV::validate(raw_instruction))
    {
        auto instruction = JLV(raw_instruction);
        if (status2.negative)
        {
            status2.membank = instruction.argument();
            pc = 0;
        }
    }

    else if (JEV::validate(raw_instruction))
    {
        auto instruction = JEV(raw_instruction);
        if (status2.zero)
        {
            status2.membank = instruction.argument();
            pc = 0;
        }
    }

    else if (JGV::validate(raw_instruction))
    {
        auto instruction = JGV(raw_instruction);
        if (!status2.negative && !status2.zero)
        {
            status2.membank = instruction.argument();
            pc = 0;
        }
    }

    else if (JMP::validate(raw_instruction))
    {
        auto instruction = JMP(raw_instruction);
        status2.membank = instruction.argument();
        pc = 0;
    }

    else if (LSH::validate(raw_instruction))
    {
        auto instruction = LSH(raw_instruction);
        uint8_t amount = instruction.argument() >> 1;

        val <<= amount;

        if (instruction.argument() & 1)
        {
            sync();
        }
    }

    else if (RSH::validate(raw_instruction))
    {
        auto instruction = RSH(raw_instruction);
        uint8_t amount = instruction.argument() >> 1;

        val >>= amount;

        if (instruction.argument() & 1)
        {
            sync();
        }
    }

    else if (CAD::validate(raw_instruction))
    {
        auto instruction = CAD(raw_instruction);

        auto result = status2.negative ? -val : val + static_cast<uint16_t>(instruction.argument() >> 1);

        status2.carry = result > std::numeric_limits<decltype(val)>().max();
        val = static_cast<uint8_t>(result);

        if (instruction.argument() & 1)
        {
            sync();
        }
    }

    else if (CSU::validate(raw_instruction))
    {
        auto instruction = CSU(raw_instruction);

        // TODO: Reuse code
        auto result = status2.negative ? -val : val - static_cast<uint16_t>(instruction.argument() >> 1);

        status2.carry = result > std::numeric_limits<decltype(val)>().max();
        val = static_cast<uint8_t>(result);

        if (instruction.argument() & 1)
        {
            sync();
        }
    }
}


void Core::sync()
{
    status1.sync = true;
}


