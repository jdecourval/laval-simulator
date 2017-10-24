#include "opcodes.h"

#include <iostream>
#include <functional>


using namespace OpCodes;


bool NOP::operator()(Registers&) const
{
    return true;
}

bool SYN::operator()(Registers& registers) const
{
    sync(registers);

    return true;
}

bool CTC::operator()(Registers& registers) const
{
    registers.status1.ctc = true;

    return true;
}

bool CTV::operator()(Registers& registers) const
{
    registers.status1.ctc = false;

    return true;
}

bool DBG::operator()(Registers& registers) const
{
    std::cout << registers << std::endl;

    return true;
}

bool HCF::operator()(Registers&) const
{
    // TODO
    return true;
}

bool MXL::operator()(Registers& registers) const
{
    if (!registers.preload)
    {
        return false;
    }

    registers.val = *registers.preload;

    if (get_argument(0))
    {
        sync(registers);
    }

    return true;
}

bool MXA::operator()(Registers& registers) const
{
    if (!registers.preload)
    {
        return false;
    }

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

    return true;
}

bool MXS::operator()(Registers& registers) const
{
    if (!registers.preload)
    {
        return false;
    }

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

    return true;
}

bool MUX::operator()(Registers& registers) const
{
    registers.status1.mux = get_argument(0);

    return true;
}

bool LCL::operator()(Registers& registers) const
{
    registers.val = static_cast<uint8_t>((registers.val & 0xf0) | get_argument(0));

    return true;
}

bool LCH::operator()(Registers& registers) const
{
    registers.val = static_cast<uint8_t>((get_argument(0) << 4) | (registers.val & 0x0f));

    return true;
}

bool JLV::operator()(Registers& registers) const
{
    if (registers.status2.negative)
    {
        registers.status2.membank = get_argument(0);
        registers.pc = 0;
    }

    return true;
}

bool JEV::operator()(Registers& registers) const
{
    if (registers.val == 0)
    {
        registers.status2.membank = get_argument(0);
        registers.pc = 0;
    }

    return true;
}

bool JGV::operator()(Registers& registers) const
{
    if (!registers.status2.negative && registers.val != 0)
    {
        registers.status2.membank = get_argument(0);
        registers.pc = 0;
    }

    return true;
}

bool JMP::operator()(Registers& registers) const
{
    registers.status2.membank = get_argument(0);
    registers.pc = 0;

    return true;
}

bool LLS::operator()(Registers& registers) const
{
    auto result = registers.val << get_argument(0);
    registers.val = static_cast<uint8_t>(result);

    // Using our powerful 32/64 bits computer we can easily check for overflow
    // Carry is true if the bit next to the MSB of the register is true
    registers.status2.carry = (result & (std::numeric_limits<decltype(registers.val)>::max() + 1)) > 0;

    if (get_argument(1))
    {
        sync(registers);
    }

    return true;
}

bool RLS::operator()(Registers& registers) const
{
    // There is no way in C++ to check right overflow, it needs to be computed
    // Shift by one less and check if there is a bit at LSB
    registers.status2.carry = get_argument(0) > 0 && (registers.val >> (get_argument(0) - 1)) & 1;

    registers.val = static_cast<uint8_t>(registers.val >> get_argument(0));

    if (get_argument(1))
    {
        sync(registers);
    }

    return true;
}

bool CAD::operator()(Registers& registers) const
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

    return true;
}

bool CSU::operator()(Registers& registers) const
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

    return true;
}
