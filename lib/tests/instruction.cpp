#include "catch.hpp"
#include "instruction.h"

// Test argument settings

template<uint8_t... ArgSizes>
struct TestInstruction : Instruction<ArgSizes...>
{
    using Instruction<ArgSizes...>::Instruction;

    void operator()(Registers&) const override
    {

    }
};

TEST_CASE("No argument and get_argument")
{
    TestInstruction<> instruction;

    REQUIRE_THROWS_AS(instruction.get_argument(0), const std::out_of_range&);
    REQUIRE_THROWS_AS(instruction.get_argument(1), const std::out_of_range&);
}

TEST_CASE("Valid load from initializer_list and get_argument")
{
    SECTION("One argument")
    {
        TestInstruction<1> instruction({1});
        REQUIRE_THROWS_AS(instruction.get_argument(2), const std::out_of_range&);
        REQUIRE_THROWS_AS(instruction.get_argument(3), const std::out_of_range&);

        REQUIRE(instruction.get_argument(0) == 1);
    }

    SECTION("Three arguments")
    {
        TestInstruction<3, 2, 1> instruction({4, 2, 1});
        REQUIRE_THROWS_AS(instruction.get_argument(3), const std::out_of_range&);
        REQUIRE_THROWS_AS(instruction.get_argument(4), const std::out_of_range&);

        REQUIRE(instruction.get_argument(0) == 4);
        REQUIRE(instruction.get_argument(1) == 2);
        REQUIRE(instruction.get_argument(2) == 1);
    }
}

TEST_CASE("Valid load from raw value and get_argument")
{
    SECTION("Single bit arguments")
    {
        TestInstruction<1, 1, 1, 1, 1, 1, 1, 1> instruction(std::byte{0xaa});

        REQUIRE(instruction.get_argument(0) == 0);
        REQUIRE(instruction.get_argument(1) == 1);
        REQUIRE(instruction.get_argument(2) == 0);
        REQUIRE(instruction.get_argument(3) == 1);
        REQUIRE(instruction.get_argument(4) == 0);
        REQUIRE(instruction.get_argument(5) == 1);
        REQUIRE(instruction.get_argument(6) == 0);
        REQUIRE(instruction.get_argument(7) == 1);
    }

    SECTION("Two bits arguments")
    {
        TestInstruction<2, 2, 2, 2> instruction(std::byte{0xb1});

        REQUIRE(instruction.get_argument(0) == 0b01);
        REQUIRE(instruction.get_argument(1) == 0b00);
        REQUIRE(instruction.get_argument(2) == 0b11);
        REQUIRE(instruction.get_argument(3) == 0b10);
    }
}

TEST_CASE("Out of range load from raw value")
{
    REQUIRE_THROWS_AS(TestInstruction<3>(std::byte{8}), const std::out_of_range&);

    using WorkAroundTemplateInMacro = TestInstruction<1, 2>;
    REQUIRE_THROWS_AS(WorkAroundTemplateInMacro(std::byte{8}), const std::out_of_range&);
}

TEST_CASE("Out of range initializer_list argument")
{
    REQUIRE_THROWS_AS(TestInstruction<1>({2}), const std::out_of_range&);
    REQUIRE_THROWS_AS(TestInstruction<1>({200}), const std::out_of_range&);
    REQUIRE_THROWS_AS(TestInstruction<3>({8}), const std::out_of_range&);
}
