#include <catch.hpp>
#include "assembler.h"

TEST_CASE("Assembler")
{
    auto stream = std::istringstream(R"(
.cores 1, 1, 1
.mem_number 3
.mem_size 3
.core_to_mem 2

1:
    MUX 1, 1, 1
    MUX 1, 2, 1
    MUX 1, 1, 0

2:
    ; Comment
    LCL 2
    LCH 1
    HLT
)");

    SECTION("parse")
    {
        auto [ast, settings] = Assembler::parse(stream);

        REQUIRE(ast.at(1).size() == 3);
        REQUIRE(ast.at(2).size() == 3);

        {
            auto &instruction = ast.at(1).at(0);
            REQUIRE(instruction.first == "MUX");
            REQUIRE(instruction.second == std::vector<uint8_t>({1, 1, 1}));
        }

        {
            auto &instruction = ast.at(1).at(1);
            REQUIRE(instruction.first == "MUX");
            REQUIRE(instruction.second == std::vector<uint8_t>({1, 2, 1}));
        }

        {
            auto &instruction = ast.at(1).at(2);
            REQUIRE(instruction.first == "MUX");
            REQUIRE(instruction.second == std::vector<uint8_t>({1, 1, 0}));
        }

        {
            auto &instruction = ast.at(2).at(0);
            REQUIRE(instruction.first == "LCL");
            REQUIRE(instruction.second == std::vector({uint8_t{2}}));
        }

        {
            auto &instruction = ast.at(2).at(2);
            REQUIRE(instruction.first == "HLT");
            REQUIRE(instruction.second.empty());
        }

        SECTION("assemble")
        {
            auto output = std::ostringstream();
            Assembler::assemble(ast, settings, output);
            auto binary = output.str();

            REQUIRE(binary.size() == 28);

            REQUIRE(binary.at(0) == 1);
            REQUIRE(binary.at(1) == 0);
            REQUIRE(binary.at(2) == 1);
            REQUIRE(binary.at(3) == 0);
            REQUIRE(binary.at(4) == 1);
            REQUIRE(binary.at(5) == 0);
            REQUIRE(binary.at(6) == 0);
            REQUIRE(binary.at(7) == 0);
            REQUIRE(binary.at(8) == 3);
            REQUIRE(binary.at(9) == 0);
            REQUIRE(binary.at(10) == 0);
            REQUIRE(binary.at(11) == 0);
            REQUIRE(binary.at(12) == 3);
            REQUIRE(binary.at(13) == 0);
            REQUIRE(binary.at(14) == 0);
            REQUIRE(binary.at(15) == 0);
            REQUIRE(binary.at(16) == 1);
            REQUIRE(binary.at(17) == 2);
            REQUIRE(binary.at(18) == 2);
            REQUIRE(binary.at(19) == 3);
            REQUIRE(binary.at(20) == 43);
            REQUIRE(binary.at(21) == 58);
            REQUIRE(binary.at(22) == 6);
            REQUIRE(binary.at(23) == 1);
            REQUIRE(binary.at(24) == 3);
            REQUIRE(binary.at(25) == 27);
            REQUIRE(binary.at(26) == 30);
            REQUIRE(binary.at(27) == 18);
            // TODO: Actually the order of the membank is not predictable because of the unordered_map
            // This may break this test and other

            SECTION("load_binary")
            {
                auto binary_stream = std::istringstream(binary);
                auto cpu = Assembler::load_binary(binary_stream);
                auto result = cpu.start(std::cin, std::cout);
                REQUIRE(result == 18);
            }
        }
    }
}
