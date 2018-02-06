#include <catch.hpp>

#include <assembler.h>
#include <cmath>


TEST_CASE("Log2 ASM")
{
    using namespace std::chrono_literals;

    auto buffer = std::stringstream();
    auto assembly_input = std::ifstream("lib/tests/integration/log2.laval", std::ios::in | std::ios::binary);
    Assembler::preprocess(assembly_input, buffer);
    auto [ast, settings] = Assembler::parse(buffer);

    auto output = std::stringstream();
    output.seekg(0);

    Assembler::assemble(ast, settings, output);

    auto cpu = Assembler::load_binary(output);

    for (auto a = 5_u8; a < 0xff_u8; a++)
    {
        std::istringstream input("0," + std::to_string(a));
        auto answer = static_cast<int>(cpu.start(input, std::cout));
        INFO(a);
        REQUIRE(answer == std::floor(std::log2(a)));
    }
}
