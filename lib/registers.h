#ifndef SIMULATOR_REGISTERS_H
#define SIMULATOR_REGISTERS_H

#include <optional>

struct Registers
{
    bool operator==(const Registers& other) const
    {
        return std::tie(val, preload, pc, status1, status2) == std::tie(other.val, other.preload, other.pc, other.status1, other.status2);
    }

    struct Status1
    {
        // 3 bits are needed for 3D or 5 bits for 4D if we ignore diagonals
        uint8_t mux: 3;
        bool ctc: 1;
        bool sync: 1;

        bool operator==(const Status1& other) const
        {
            return std::tie(mux, ctc, sync) == std::tie(other.mux, other.ctc, other.sync);
        }
    };

    struct Status2
    {
        // Bitfield could have been used but are pretty difficult to deal with
        // since its then impossible to take member's address
        uint8_t membank; //: 4;
        bool carry; //: 1;
        bool negative; //: 1;
        bool overflow;// : 1;
        bool zero; //: 1;

        bool operator==(const Status2& other) const
        {
            return std::tie(membank, carry, negative, overflow, zero) == std::tie(other.membank, other.carry, other.negative, other.overflow, other.zero);
        }
    };

    // Registers
    uint8_t val{};
    std::optional<uint8_t> preload;
    uint8_t pc{};
    Status1 status1{};
    Status2 status2{};
};

#endif //SIMULATOR_REGISTERS_H
