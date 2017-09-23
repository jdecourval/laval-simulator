#ifndef SIMULATOR_CORE_H
#define SIMULATOR_CORE_H

#include <array>
#include <vector>
#include <cstdint>
#include <functional>
#include "opcodes.h"
#include "settings.h"
#include "memory.h"


class CoreArray;


class Core
{
    static uint8_t devnull;

    struct Status1
    {
        // 3 bits are needed for 3D or 5 bits for 4D if we ignore diagonals
        uint8_t mux: 3;
        bool ctc: 1;
        bool sync: 1;
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

    };

public:
    Core();

    void check();

    void link(CoreArray *cores, size_t id, Memory_t *mem);

    void step1();

    void step2();

//    std::array<Instruction, 16> mem;

private:
    Core(Core&) = delete;

    Core& operator=(Core&) = delete;

    void sync();

    Memory_t *mem;

    // Registers
    uint8_t val;
    std::optional<uint8_t> preload;
    uint8_t pc;
    Status1 status1;
    Status2 status2;

    size_t id;
    CoreArray *cores;
};


#endif //SIMULATOR_CORE_H
