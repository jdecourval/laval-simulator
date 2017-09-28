#include "registers.h"

bool Registers::operator==(const Registers& other) const
{
    return std::tie(val, preload, pc, status1, status2) ==
           std::tie(other.val, other.preload, other.pc, other.status1, other.status2);
}

bool Registers::Status1::operator==(const Registers::Status1& other) const
{
    return std::tie(mux, ctc, sync) == std::tie(other.mux, other.ctc, other.sync);
}

bool Registers::Status2::operator==(const Registers::Status2& other) const
{
    return std::tie(membank, carry, negative, overflow) ==
           std::tie(other.membank, other.carry, other.negative, other.overflow);
}
