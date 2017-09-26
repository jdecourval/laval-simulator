#include "core.h"
#include "catch.hpp"

TEST_CASE("NOP")
{
    SECTION("NOP does not affect registers")
    {
        Registers registers;
        auto registers_copy = registers;

        REQUIRE(registers_copy == registers);

        NOP instruction;
        instruction(registers);

        REQUIRE(registers_copy == registers);
    }
}

TEST_CASE("CAD")
{
    Registers registers;

    SECTION("No carry, initially zero")
    {
        CAD instruction(2);
        instruction(registers);

        REQUIRE(registers.val == 2);
        REQUIRE(!registers.status2.carry);
        REQUIRE(!registers.status2.negative);
    }

    SECTION("Carry, initially positive")
    {
        registers.val = 230;

        CAD instruction(31);
        instruction(registers);

        REQUIRE(registers.val == static_cast<uint8_t>(230 + 31));
        REQUIRE(registers.status2.carry);
        REQUIRE(!registers.status2.negative);
    }

    SECTION("Initially negative")
    {
        registers.val = static_cast<uint8_t>(-5);
        registers.status2.negative = true;
        REQUIRE(registers.val == static_cast<uint8_t>(-5));

        SECTION("Still negative")
        {
            CAD instruction(3);
            instruction(registers);

            REQUIRE(static_cast<int>(registers.val) == static_cast<uint8_t>(-2));
            REQUIRE(!registers.status2.carry);
            REQUIRE(registers.status2.negative);
        }

        SECTION("Then zero")
        {
            CAD instruction(5);
            instruction(registers);

            REQUIRE(registers.val == 0);
            REQUIRE(!registers.status2.carry);
            REQUIRE(!registers.status2.negative);
        }

        SECTION("Then positive")
        {
            CAD instruction(6);
            instruction(registers);

            REQUIRE(registers.val == 1);
            REQUIRE(!registers.status2.carry);
            REQUIRE(!registers.status2.negative);
        }
    }

}
