#include <iostream>

#include "opcodes.h"


void NOP::operator()(Registers&) const
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
    std::cout << "pc: " << std::hex << (registers.pc & 0xf) << std::endl;
    std::cout << "membank: " << std::hex << registers.status2.membank << std::endl;
    std::cout << "val: " << std::hex << registers.val << std::endl;
    if (registers.preload)
    {
        std::cout << "mux: " << std::hex << registers.status1.mux << " -> " << *registers.preload << std::endl;
    }
    else
    {
        std::cout << "mux: " << std::hex << registers.status1.mux << std::endl;
    }
    std::cout << "sync: " << std::hex << registers.status1.sync << std::endl;
    std::cout << "ctc: " << std::hex << registers.status1.ctc << std::endl;
    std::cout << "negative: " << std::hex << registers.status2.negative << std::endl;
    std::cout << "carry: " << std::hex << registers.status2.carry << std::endl;
}

void HCF::operator()(Registers&) const
{
    // TODO
}

void MXL::operator()(Registers& registers) const
{
    registers.val = *registers.preload;

    if (get_argument(0))
    {
        sync(registers);
    }
}

void MXA::operator()(Registers& registers) const
{
    // TODO: Do not rely on negative flag
    // TODO: Reuse code in CAD, MXS and CSU
    auto first_term = registers.status2.negative ? static_cast<int16_t>(static_cast<int8_t>(registers.val)) : registers.val;
    auto second_term = registers.preload_negative ? static_cast<int16_t>(static_cast<int8_t>(*registers.preload)) : *registers.preload;
    auto result = first_term + second_term;

    // TODO: Should negative signed integer overflow set carry flag ?
    registers.status2.carry = checkcarry(result);
    registers.val = static_cast<uint8_t>(result);
    registers.status2.negative = result < 0;

    if (get_argument(0))
    {
        sync(registers);
    }
}

void MXS::operator()(Registers& registers) const
{
    auto first_term = registers.status2.negative ? static_cast<int16_t>(static_cast<int8_t>(registers.val)) : registers.val;
    auto second_term = registers.preload_negative ? static_cast<int16_t>(static_cast<int8_t>(*registers.preload)) : *registers.preload;
    auto result = first_term - second_term;

    registers.status2.carry = checkcarry(result);
    registers.val = static_cast<uint8_t>(result);
    registers.status2.negative = result < 0;

    if (get_argument(0))
    {
        sync(registers);
    }
}

void MUX::operator()(Registers& registers) const
{

    registers.status1.mux = get_argument(0);
}

void LCL::operator()(Registers& registers) const
{

    registers.val = static_cast<uint8_t>((registers.val & 0xf0) | get_argument(0));
}

void LCH::operator()(Registers& registers) const
{

    registers.val = static_cast<uint8_t>((get_argument(0) << 4) | (registers.val & 0x0f));
}

void JLV::operator()(Registers& registers) const
{
    if (registers.status2.negative)
    {
        registers.status2.membank = get_argument(0);
        registers.pc = 0;
    }
}

void JEV::operator()(Registers& registers) const
{
    if (registers.val == 0)
    {
        registers.status2.membank = get_argument(0);
        registers.pc = 0;
    }
}

void JGV::operator()(Registers& registers) const
{
    if (!registers.status2.negative && registers.val != 0)
    {
        registers.status2.membank = get_argument(0);
        registers.pc = 0;
    }
}

void JMP::operator()(Registers& registers) const
{
    registers.status2.membank = get_argument(0);
    registers.pc = 0;
}

void LSH::operator()(Registers& registers) const
{
    uint8_t amount = get_argument(0) >> 1;

    registers.val <<= amount;

    if (get_argument(1))
    {
        sync(registers);
    }

}

void RSH::operator()(Registers& registers) const
{
    uint8_t amount = get_argument(0) >> 1;

    registers.val >>= amount;

    if (get_argument(1))
    {
        sync(registers);
    }
}

void CAD::operator()(Registers& registers) const
{
    auto first_term = registers.status2.negative ? static_cast<int16_t>(static_cast<int8_t>(registers.val)) : registers.val;
    auto second_term = get_argument(0);
    auto result = first_term + second_term;

    registers.status2.carry = checkcarry(result);
    registers.val = static_cast<uint8_t>(result);
    registers.status2.negative = result < 0;

    if (get_argument(1))
    {
        sync(registers);
    }
}

void CSU::operator()(Registers& registers) const
{
    auto first_term = registers.status2.negative ? static_cast<int16_t>(static_cast<int8_t>(registers.val)) : registers.val;
    auto second_term = get_argument(0);
    auto result = first_term - second_term;

    registers.status2.carry = checkcarry(result);
    // TODO: Undefined behaviour, value cannot be represented in the destination type
    registers.val = static_cast<uint8_t>(result);

    if (get_argument(1))
    {
        sync(registers);
    }
}
