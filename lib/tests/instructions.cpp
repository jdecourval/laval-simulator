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

TEST_CASE("SYN")
{
    SECTION("SYN sets sync flag")
    {
        Registers registers;
        REQUIRE(!registers.status1.sync);

        SYN instruction;
        instruction(registers);

        REQUIRE(registers.status1.sync);
    }
}

TEST_CASE("CTC")
{
    SECTION("CTC sets sync ctc flag")
    {
        Registers registers;
        REQUIRE(!registers.status1.ctc);

        CTC instruction;
        instruction(registers);

        REQUIRE(registers.status1.ctc);

        instruction(registers);

        REQUIRE(registers.status1.ctc);
    }
}

TEST_CASE("CTV")
{
    SECTION("CTV unsets sync ctc flag")
    {
        Registers registers;
        registers.status1.ctc = true;

        CTV instruction;
        instruction(registers);

        REQUIRE(!registers.status1.ctc);

        instruction(registers);

        REQUIRE(!registers.status1.ctc);
    }
}

TEST_CASE("MXL")
{
    Registers registers;
    registers.preload = 5;

    SECTION("Without sync")
    {
        MXL instruction({0});
        instruction(registers);

        REQUIRE(registers.preload == 5);
        REQUIRE(registers.val == registers.preload);
        REQUIRE(!registers.status1.sync);
    }

    SECTION("With sync")
    {
        MXL instruction({1});
        instruction(registers);

        REQUIRE(registers.preload == 5);
        REQUIRE(registers.val == registers.preload);
        REQUIRE(registers.status1.sync);
    }
}

TEST_CASE("CAD")
{
    Registers registers;

    SECTION("No sync")
    {
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
            registers.val = 250;

            CAD instruction({15, 0});
            instruction(registers);

            REQUIRE(static_cast<int>(registers.val) == static_cast<int>(static_cast<uint8_t>(250 + 15)));
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
                CAD instruction({3, 0});
                instruction(registers);

                REQUIRE(static_cast<int>(registers.val) == static_cast<uint8_t>(-2));
                REQUIRE(!registers.status2.carry);
                REQUIRE(registers.status2.negative);
            }

            SECTION("Then zero")
            {
                CAD instruction({5, 0});
                instruction(registers);

                REQUIRE(registers.val == 0);
                REQUIRE(!registers.status2.carry);
                REQUIRE(!registers.status2.negative);
            }

            SECTION("Then positive")
            {
                CAD instruction({6, 0});
                instruction(registers);

                REQUIRE(registers.val == 1);
                REQUIRE(!registers.status2.carry);
                REQUIRE(!registers.status2.negative);
            }
        }

        REQUIRE(!registers.status1.sync);
    }

    SECTION("Sync")
    {

    }
}
