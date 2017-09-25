#include <iostream>
#include "opcodes.h"

void NOP::operator()(Registers& registers) const
{

}

void SYN::operator()(Registers& registers) const
{
    sync(registers);
}

void CTC::operator()(Registers& registers) const
{
    registers.status1.ctc = true;
}

void CTV::operator()(Registers& registers) const
{
    registers.status1.ctc = false;
}

void DBG::operator()(Registers& registers) const
{
//    std::cout << "id: " << id << std::endl;
    std::cout << "pc: " << (registers.pc & 0xf) << std::endl;
    std::cout << "membank: " << static_cast<int>(registers.status2.membank) << std::endl;
    std::cout << "val: " << static_cast<int>(registers.val) << std::endl;
    std::cout << "mux: " << static_cast<int>(registers.status1.mux) << " -> " << static_cast<int>(*registers.preload) << std::endl;
}

void HCF::operator()(Registers& registers) const
{
    // TODO
}

void MXL::operator()(Registers& registers) const
{
    registers.val = *registers.preload;

    if (argument() & 1)
    {
        sync(registers);
    }
}

void MXA::operator()(Registers& registers) const
{

    registers.val += *registers.preload;

    if (argument() & 1)
    {
        sync(registers);
    }
}

void MXS::operator()(Registers& registers) const
{
    registers.val -= *registers.preload;

    if (argument() & 1)
    {
        sync(registers);
    }
}

void MUX::operator()(Registers& registers) const
{

    registers.status1.mux = argument();
}

void LCL::operator()(Registers& registers) const
{

    registers.val = static_cast<uint8_t>((registers.val & 0xf0) | argument());
}

void LCH::operator()(Registers& registers) const
{

    registers.val = static_cast<uint8_t>((argument() << 4) | (registers.val & 0x0f));
}

void JLV::operator()(Registers& registers) const
{
    if (registers.status2.negative)
    {
        registers.status2.membank = argument();
        registers.pc = 0;
    }
}

void JEV::operator()(Registers& registers) const
{
    if (registers.status2.zero)
    {
        registers.status2.membank = argument();
        registers.pc = 0;
    }
}

void JGV::operator()(Registers& registers) const
{
    if (!registers.status2.negative && !registers.status2.zero)
    {
        registers.status2.membank = argument();
        registers.pc = 0;
    }
}

void JMP::operator()(Registers& registers) const
{
    registers.status2.membank = argument();
    registers.pc = 0;
}

void LSH::operator()(Registers& registers) const
{
    uint8_t amount = argument() >> 1;

    registers.val <<= amount;

    if (argument() & 1)
    {
        sync(registers);
    }

}

void RSH::operator()(Registers& registers) const
{
    uint8_t amount = argument() >> 1;

    registers.val >>= amount;

    if (argument() & 1)
    {
        sync(registers);
    }
}

void CAD::operator()(Registers& registers) const
{
    auto result = registers.status2.negative ? -registers.val : registers.val + static_cast<uint16_t>(argument() >> 1);

    registers.status2.carry = result > std::numeric_limits<decltype(registers.val)>().max();
    registers.val = static_cast<uint8_t>(result);

    if (argument() & 1)
    {
        sync(registers);
    }
}

void CSU::operator()(Registers& registers) const
{
    // TODO: Reuse code
    auto result = registers.status2.negative ? -registers.val : registers.val - static_cast<uint16_t>(argument() >> 1);

    registers.status2.carry = result > std::numeric_limits<decltype(registers.val)>().max();
    registers.val = static_cast<uint8_t>(result);

    if (argument() & 1)
    {
        sync(registers);
    }
}
