#include <catch.hpp>

#include <cpu.h>
#include <direction.h>
#include <opcodes.h>


// Compute c = a * b, a > b
// From https://en.wikipedia.org/wiki/Bitwise_operation
// Algo:
/*
c ← 0
while b ≠ 0
    if (b and 1) ≠ 0
        c ← c + a
    left shift a by 1
    right shift b by 1
return c
 */
TEST_CASE("Multiplication")
{
    using namespace OpCodes;
    using namespace std::chrono_literals;

    Settings settings{};
    settings.dimensions = {1, 2, 3};
    settings.bank_number = 9;
    settings.bank_size = 10;

    Memory memory{settings};
    Cpu cpu{settings};

    // Map four cores to four different membanks
    std::vector<size_t> map {0, 5, 2, 8, 7, 8};

    // Core 0: shift a
    // Membank 0, init a
    memory.at(0).at(0) = cpu.dump(LCL({6}));
    memory.at(0).at(1) = cpu.dump(JMP({1}));

    // Membank 1, shift a
    memory.at(1).at(0) = cpu.dump(SYN());
    memory.at(1).at(1) = cpu.dump(LLS({1}));
    memory.at(1).at(2) = cpu.dump(JMP({1}));


    // Core 1: return value to add
    // Membank 5, b & 1 != 0: return a
    memory.at(5).at(0) = cpu.dump(MUX({Direction({Direction::CURRENT, Direction::CURRENT, Direction::AFTER}).dump()}));
    memory.at(5).at(1) = cpu.dump(MXL({0}));
    memory.at(5).at(2) = cpu.dump(CAN({1}));
    memory.at(5).at(4) = cpu.dump(MUX({Direction({Direction::CURRENT, Direction::CURRENT, Direction::BEFORE}).dump()}));
    memory.at(5).at(5) = cpu.dump(JEZ({6}));
    memory.at(5).at(6) = cpu.dump(MXL({0}));    // TODO: Check instructions with integrated sync
    memory.at(5).at(7) = cpu.dump(SYN());
    memory.at(5).at(8) = cpu.dump(JMP({5}));

    // Membank 6, return 0
    memory.at(6).at(0) = cpu.dump(LCL({0}));
    memory.at(6).at(1) = cpu.dump(LCH({0}));
    memory.at(6).at(2) = cpu.dump(MXD());
    memory.at(6).at(3) = cpu.dump(SYN());
    memory.at(6).at(4) = cpu.dump(JMP({5}));


    // Core 2: loop on b and shift b
    // Membank 2, init b
    memory.at(2).at(0) = cpu.dump(LCL({3}));
    memory.at(2).at(1) = cpu.dump(MUX({Direction({Direction::CURRENT, Direction::AFTER, Direction::BEFORE}).dump()}));
    memory.at(2).at(2) = cpu.dump(JMP({3}));

    // Membank 3, while b != 0, shift
    memory.at(3).at(0) = cpu.dump(JEZ({4}));
    memory.at(3).at(1) = cpu.dump(SYN());
    memory.at(3).at(2) = cpu.dump(MXD());
    memory.at(3).at(3) = cpu.dump(RLS({1}));
    memory.at(3).at(4) = cpu.dump(JMP({3}));

    // Membank 4: halt

    memory.at(4).at(0) = cpu.dump(MXL({0}));
    memory.at(4).at(1) = cpu.dump(MXL({0}));    // TODO: This is cheating
    memory.at(4).at(2) = cpu.dump(MXL({0}));    // TODO: This is cheating
    memory.at(4).at(3) = cpu.dump(DBG());
    memory.at(4).at(4) = cpu.dump(HLT());


    // Core 4: accumulate c
    // Membank 7, init c
    memory.at(7).at(0) = cpu.dump(MUX({Direction({Direction::CURRENT, Direction::BEFORE, Direction::CURRENT}).dump()}));  // TODO: Put in another memory bank
    memory.at(7).at(1) = cpu.dump(MXA({0}));
    memory.at(7).at(3) = cpu.dump(SYN());
    memory.at(7).at(4) = cpu.dump(JMP({7}));

    cpu.link_memory(std::move(memory), std::move(map));
    cpu.start(100ms);
}
