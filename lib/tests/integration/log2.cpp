#include <catch.hpp>

#include <assembler.h>
#include <cmath>


TEST_CASE("Log2 ASM")
{
    using namespace std::chrono_literals;

    auto buffer = std::stringstream();
    auto assembly_input = std::ifstream("lib/tests/integration/log2.laval", std::ios::in | std::ios::out);
    Assembler::preprocess(assembly_input, buffer);
    auto [ast, settings, variables] = Assembler::build_ast(buffer);

    auto output = std::stringstream();

    Assembler::assemble(ast, settings, variables, output);

    auto cpu = Assembler::load_binary(output);

    for (auto a = 5_u8; a < 0xffu; a++)
    {
        auto test = cpu.start(0s, std::vector({a}));
        auto answer = static_cast<int>(test);
        INFO(a);
        REQUIRE(answer == std::floor(std::log2(a)));
    }
}
