#include "registers.h"

#include "opcodes.h"

#include <ostream>


bool Registers::operator==(const Registers& other) const
{
    return std::tie(val, preload, pc, status1, status2) ==
           std::tie(other.val, other.preload, other.pc, other.status1, other.status2);
}

bool Registers::Status1::operator==(const Registers::Status1& other) const
{
    return std::tie(mux, ctc, sync) == std::tie(other.mux, other.ctc, other.sync);
}

Registers::Status1::Status1()
{
    mux = OpCodes::MUX({Direction::CURRENT, Direction::CURRENT, Direction::CURRENT}).dump_args();
    ctc = false;
    sync = false;
}

bool Registers::Status2::operator==(const Registers::Status2& other) const
{
    return std::tie(membank, carry, negative, overflow, unlock) ==
           std::tie(other.membank, other.carry, other.negative, other.overflow, other.unlock);
}


std::ostream& operator<<(std::ostream& os, const Registers& registers)
{
    os << "id: " << std::hex << registers.id << std::endl;
    os << "pc: " << std::hex << static_cast<int>(registers.pc) << std::endl;
    os << "membank: " << std::hex << static_cast<int>(registers.status2.membank) << std::endl;
    os << "val: " << std::hex << static_cast<int>(registers.val) << std::endl;

    if (registers.preload)
    {
        os << "mux: " << std::hex << static_cast<int>(registers.status1.mux) << " -> " << *registers.preload << std::endl;
    }
    else
    {
        os << "mux: " << std::hex << static_cast<int>(registers.status1.mux) << std::endl;
    }

    os << "sync: " << std::hex << registers.status1.sync << std::endl;
    os << "unlock: " << std::hex << registers.status2.unlock << std::endl;
    os << "ctc: " << std::hex << registers.status1.ctc << std::endl;
    os << "negative: " << std::hex << registers.status2.negative << std::endl;
    os << "carry: " << std::hex << registers.status2.carry << std::endl;

    return os;
}
