#ifndef SIMULATOR_REGISTERS_H
#define SIMULATOR_REGISTERS_H

#include <optional>
#include <tuple>


struct Registers
{
    bool operator==(const Registers& other) const;


    struct Status1
    {
        // 3 bits are needed for 3D or 5 bits for 4D if we ignore diagonals
        uint8_t mux: 3;
        bool ctc: 1;
        bool sync: 1;

        bool operator==(const Status1& other) const;
    };


    struct Status2
    {
        // Bitfield could have been used but are pretty difficult to deal with
        // since its then impossible to take member's address
        uint8_t membank; //: 4;
        bool carry; //: 1;
        bool negative; //: 1;
        bool overflow;// : 1;
//        bool notzero; //: 1;  // Its not really worth it

        bool operator==(const Status2& other) const;
    };


    // Registers
    uint8_t val{};
    std::optional<uint8_t> preload;
    uint8_t pc{};
    Status1 status1{};
    Status2 status2{};
};


#endif //SIMULATOR_REGISTERS_H
