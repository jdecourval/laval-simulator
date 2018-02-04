#include <catch.hpp>

#include <assembler.h>
#include <opcodes.h>


// Compute c = a * b
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
uint8_t multiplication(uint8_t a, uint8_t b)
{
    using namespace OpCodes;
    using namespace std::chrono_literals;

    Settings settings{};
    settings.dimensions = {1, 2, 3};
    settings.bank_number = 10;
    settings.bank_size = 10;

    Memory memory{settings};
    Cpu cpu{settings};

    // Map four cores to four different membanks
    std::vector<MemoryInterface::size_type> map {0, 2, 4, 9, 7, 9};

    // Core 0: shift a
    // Membank 0, init a
    memory.at(0).at(0) = cpu.dump(LCL({a}));
    memory.at(0).at(1) = cpu.dump(JMP({1}));

    // Membank 1, shift a
    memory.at(1).at(0) = cpu.dump(SYN());
    memory.at(1).at(1) = cpu.dump(LLS({1}));
    memory.at(1).at(2) = cpu.dump(JMP({1}));


    // Core 1: return value to add
    // Membank 5, b & 1 != 0: return a
    memory.at(2).at(0) = cpu.dump(MUX({Direction::CURRENT, Direction::CURRENT, Direction::AFTER}));
    memory.at(2).at(1) = cpu.dump(MXL());
    memory.at(2).at(2) = cpu.dump(CAN({1}));
    memory.at(2).at(4) = cpu.dump(MUX({Direction::CURRENT, Direction::CURRENT, Direction::BEFORE}));
    memory.at(2).at(5) = cpu.dump(JEZ({3}));
    memory.at(2).at(6) = cpu.dump(MXL());
    memory.at(2).at(7) = cpu.dump(SYN());
    memory.at(2).at(8) = cpu.dump(JMP({2}));

    // Membank 6, return 0
    memory.at(3).at(0) = cpu.dump(MXD());
    memory.at(3).at(1) = cpu.dump(LCL({0}));
    memory.at(3).at(2) = cpu.dump(LCH({0}));
    memory.at(3).at(3) = cpu.dump(SYN());
    memory.at(3).at(4) = cpu.dump(JMP({2}));


    // Core 2: loop on b and shift b
    // Membank 2, init b
    memory.at(4).at(0) = cpu.dump(LCL({b}));
    memory.at(4).at(1) = cpu.dump(MUX({Direction::CURRENT, Direction::AFTER, Direction::BEFORE}));
    memory.at(4).at(2) = cpu.dump(JMP({5}));

    // Membank 3, while b != 0, shift
    memory.at(5).at(0) = cpu.dump(JEZ({6}));
    memory.at(5).at(1) = cpu.dump(SYN());
    memory.at(5).at(2) = cpu.dump(MXD());
    memory.at(5).at(3) = cpu.dump(RLS({1}));
    memory.at(5).at(4) = cpu.dump(JMP({5}));

    // Membank 4: halt
    memory.at(6).at(0) = cpu.dump(SYN());
    memory.at(6).at(1) = cpu.dump(MXL());
    memory.at(6).at(2) = cpu.dump(HLT());


    // Core 4: accumulate c
    // Membank 7, init c
    memory.at(7).at(0) = cpu.dump(MUX({Direction::CURRENT, Direction::BEFORE, Direction::CURRENT}));  // TODO: Put in another memory bank
    memory.at(7).at(2) = cpu.dump(JMP({8}));

    // Membank 8, accumulate
    memory.at(8).at(0) = cpu.dump(MXA());
    memory.at(8).at(1) = cpu.dump(SYN());
    memory.at(8).at(2) = cpu.dump(JMP({8}));

    cpu.link_memory(std::move(memory), settings);

    return cpu.start(std::cin, std::cout);
}

//TEST_CASE("Multiplication")
//{
//    for (auto a = 0u; a <= 0b1111; a++)
//    {
//        for (auto b = 0u; b <= 0b1111; b++)
//        {
//            REQUIRE(multiplication(a, b) == a * b);
//        }
//    }
//}

TEST_CASE("Multiplication ASM")
{
    using namespace std::chrono_literals;

    auto buffer = std::stringstream();
    auto assembly_input = std::ifstream("lib/tests/integration/multiplication.laval", std::ios::binary);
    Assembler::preprocess(assembly_input, buffer);
    auto [ast, settings] = Assembler::build_ast(buffer);

    auto output = std::stringstream();

    Assembler::assemble(ast, settings, output);

    auto cpu = Assembler::load_binary(output);

    auto input_str = "0,2 2,3";
    std::istringstream input(input_str);

    auto answer = static_cast<int>(cpu.start(input, std::cout));
    REQUIRE(answer == 6);
}
