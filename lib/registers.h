#ifndef SIMULATOR_REGISTERS_H
#define SIMULATOR_REGISTERS_H

#include <optional>
#include <tuple>


struct Registers
{
    bool operator==(const Registers& other) const;


    struct Status1
    {
        // TODO: 4 bits (+ special directions) are needed for 3D if we ignore diagonals
        uint8_t mux; //: 6;
        bool ctc; //: 1;
        bool sync; //: 1;

        Status1();
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


    size_t id{};    // Implementation helper, would not be needed in a real ASIC

    // Registers
    uint8_t val{};
    std::optional<uint8_t> preload;
    bool preload_negative{false};
    uint8_t pc{};
    Status1 status1{};
    Status2 status2{};
};

std::ostream& operator<<(std::ostream& os, const Registers& registers);

#endif //SIMULATOR_REGISTERS_H
