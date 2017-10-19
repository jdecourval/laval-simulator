#include "catch.hpp"

#include "opcodes.h"
#include "core.h"


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

TEST_CASE("MXA")
{
    Registers registers;

    SECTION("No sync")
    {
        SECTION("No carry")
        {
            SECTION("Initially zero, add positive")
            {
                registers.preload = 2;
                MXA instruction({0});
                instruction(registers);

                REQUIRE(registers.val == 2);
                REQUIRE(!registers.status2.negative);
                REQUIRE(registers.preload == 2);
            }

            SECTION("Initially zero, add negative")
            {
                registers.preload = -2;
                registers.preload_negative = true;
                MXA instruction({0});
                instruction(registers);

                REQUIRE(registers.val == static_cast<uint8_t>(-2));
                REQUIRE(registers.status2.negative);
                REQUIRE(registers.preload == static_cast<uint8_t>(-2));
            }

            REQUIRE(!registers.status2.carry);
        }

        SECTION("Carry, initially positive")
        {
            registers.val = 250;
            registers.preload = 15;

            MXA instruction({0});
            instruction(registers);

            REQUIRE(static_cast<int>(registers.val) == static_cast<int>(static_cast<uint8_t>(250 + 15)));
            REQUIRE(registers.status2.carry);
            REQUIRE(!registers.status2.negative);
            REQUIRE(registers.preload == 15);
        }

        SECTION("Negative carry, initially negative")
        {
            registers.val = static_cast<uint8_t>(-127);
            registers.preload = static_cast<uint8_t>(-15);
            registers.preload_negative = true;

            MXA instruction({0});
            instruction(registers);

            REQUIRE(static_cast<int>(registers.val) == static_cast<int>(static_cast<uint8_t>(-127 - 15)));
            REQUIRE(registers.status2.carry);
            REQUIRE(!registers.status2.negative);
            REQUIRE(registers.preload == static_cast<uint8_t>(-15));
        }

        SECTION("Initially negative")
        {
            registers.val = static_cast<uint8_t>(-5);
            registers.status2.negative = true;
            REQUIRE(registers.val == static_cast<uint8_t>(-5));

            SECTION("Still negative")
            {
                registers.preload = 3;

                MXA instruction({0});
                instruction(registers);

                REQUIRE(static_cast<int>(registers.val) == static_cast<uint8_t>(-2));
                REQUIRE(!registers.status2.carry);
                REQUIRE(registers.status2.negative);
                REQUIRE(registers.preload == 3);
            }

            SECTION("Then zero")
            {
                registers.preload = 5;

                MXA instruction({0});
                instruction(registers);

                REQUIRE(registers.val == 0);
                REQUIRE(!registers.status2.carry);
                REQUIRE(!registers.status2.negative);
                REQUIRE(registers.preload == 5);
            }

            SECTION("Then positive")
            {
                registers.preload = 6;

                MXA instruction({0});
                instruction(registers);

                REQUIRE(registers.val == 1);
                REQUIRE(!registers.status2.carry);
                REQUIRE(!registers.status2.negative);
                REQUIRE(registers.preload == 6);
            }
        }

        REQUIRE(!registers.status1.sync);
    }

    SECTION("Sync")
    {
        registers.preload = 2;
        MXA instruction({1});
        instruction(registers);

        REQUIRE(registers.val == 2);
        REQUIRE(!registers.status2.carry);
        REQUIRE(!registers.status2.negative);
        REQUIRE(registers.preload == 2);
        REQUIRE(registers.status1.sync);
    }
}

TEST_CASE("MXS")
{
    Registers registers;

    SECTION("No sync")
    {
        SECTION("No carry")
        {
            SECTION("Initially zero, subtract positive")
            {
                registers.preload = 2;
                MXS instruction({0});
                instruction(registers);

                REQUIRE(registers.val == static_cast<uint8_t>(-2));
                REQUIRE(registers.status2.negative);
                REQUIRE(registers.preload == 2);
            }

            SECTION("Initially zero, subtract negative")
            {
                registers.preload = -2;
                registers.preload_negative = true;
                MXS instruction({0});
                instruction(registers);

                REQUIRE(registers.val == 2);
                REQUIRE(!registers.status2.negative);
                REQUIRE(registers.preload == static_cast<uint8_t>(-2));
            }

            REQUIRE(!registers.status2.carry);
        }

        SECTION("Carry, initially positive")
        {
            registers.val = 250;
            registers.preload = static_cast<uint8_t>(-15);
            registers.preload_negative = true;

            MXS instruction({0});
            instruction(registers);

            REQUIRE(static_cast<int>(registers.val) == static_cast<int>(static_cast<uint8_t>(250 + 15)));
            REQUIRE(registers.status2.carry);
            REQUIRE(!registers.status2.negative);
            REQUIRE(registers.preload == static_cast<uint8_t>(-15));
        }

        SECTION("Negative carry, initially negative")
        {
            registers.val = static_cast<uint8_t>(-127);
            registers.preload = 15;

            MXS instruction({0});
            instruction(registers);

            REQUIRE(static_cast<int>(registers.val) == static_cast<int>(static_cast<uint8_t>(-127 - 15)));
            REQUIRE(registers.status2.carry);
            REQUIRE(!registers.status2.negative);
            REQUIRE(registers.preload == static_cast<uint8_t>(-15));
        }

        SECTION("Initially negative")
        {
            registers.val = static_cast<uint8_t>(-5);
            registers.status2.negative = true;
            REQUIRE(registers.val == static_cast<uint8_t>(-5));

            SECTION("Still negative")
            {
                registers.preload = static_cast<uint8_t>(-3);
                registers.preload_negative = true;

                MXS instruction({0});
                instruction(registers);

                REQUIRE(static_cast<int>(registers.val) == static_cast<uint8_t>(-2));
                REQUIRE(!registers.status2.carry);
                REQUIRE(registers.status2.negative);
                REQUIRE(registers.preload == static_cast<uint8_t>(-3));
            }

            SECTION("Then zero")
            {
                registers.preload = static_cast<uint8_t>(-5);
                registers.preload_negative = true;

                MXS instruction({0});
                instruction(registers);

                REQUIRE(registers.val == 0);
                REQUIRE(!registers.status2.carry);
                REQUIRE(!registers.status2.negative);
                REQUIRE(registers.preload == static_cast<uint8_t>(-5));
            }

            SECTION("Then positive")
            {
                registers.preload = static_cast<uint8_t>(-6);
                registers.preload_negative = true;

                MXS instruction({0});
                instruction(registers);

                REQUIRE(registers.val == 1);
                REQUIRE(!registers.status2.carry);
                REQUIRE(!registers.status2.negative);
                REQUIRE(registers.preload == static_cast<uint8_t>(-6));
            }
        }

        REQUIRE(!registers.status1.sync);
    }

    SECTION("Sync")
    {
        registers.preload = 2;
        MXS instruction({1});
        instruction(registers);

        REQUIRE(registers.val == static_cast<uint8_t>(-2));
        REQUIRE(!registers.status2.carry);
        REQUIRE(registers.status2.negative);
        REQUIRE(registers.preload == 2);
        REQUIRE(registers.status1.sync);
    }
}

TEST_CASE("MUX")
{
    Registers registers;

    SECTION("Correctly set mux value")
    {
        MUX instruction({3});

        instruction(registers);

        REQUIRE(registers.status1.mux == 3);
    }
}

TEST_CASE("LCL")
{
    Registers registers;

    SECTION("Initially zero")
    {
        LCL instruction({0x05});
        instruction(registers);

        REQUIRE(registers.val == 0x05);
    }

    SECTION("Initially have some random value")
    {
        registers.val = 0xaa;

        LCL instruction({0x05});
        instruction(registers);

        REQUIRE(registers.val == 0xa5);
    }
}

TEST_CASE("LCH")
{
    Registers registers;

    SECTION("Initially zero")
    {
        LCH instruction({0x05});
        instruction(registers);

        REQUIRE(registers.val == 0x50);
    }

    SECTION("Initially have some random value")
    {
        registers.val = 0xaa;

        LCH instruction({0x05});
        instruction(registers);

        REQUIRE(registers.val == 0x5a);
    }
}

TEST_CASE("JLV")
{
    Registers registers;
    registers.pc = 1;

    SECTION("No effect if zero")
    {
        JLV instruction({4});
        instruction(registers);

        REQUIRE(registers.pc == 1);
        REQUIRE(registers.status2.membank == 0);
    }

    SECTION("No effect if greater than zero")
    {
        JLV instruction({4});
        registers.val = 2;
        instruction(registers);

        REQUIRE(registers.pc == 1);
        REQUIRE(registers.status2.membank == 0);
    }

    SECTION("Jump if negative")
    {
        JLV instruction({4});
        registers.val = 1;
        registers.status2.negative = true;
        instruction(registers);

        REQUIRE(registers.pc == 0);
        REQUIRE(registers.status2.membank == 4);
    }
}

TEST_CASE("JEV")
{
    Registers registers;
    registers.pc = 1;

    SECTION("Jump if zero")
    {
        JEV instruction({4});
        instruction(registers);

        REQUIRE(registers.pc == 0);
        REQUIRE(registers.status2.membank == 4);
    }

    SECTION("No effect if greater than zero")
    {
        JEV instruction({4});
        registers.val = 2;
        instruction(registers);

        REQUIRE(registers.pc == 1);
        REQUIRE(registers.status2.membank == 0);
    }

    SECTION("No effect if negative")
    {
        JEV instruction({4});
        registers.val = 1;
        registers.status2.negative = true;
        instruction(registers);

        REQUIRE(registers.pc == 1);
        REQUIRE(registers.status2.membank == 0);
    }
}

TEST_CASE("JGV")
{
    Registers registers;
    registers.pc = 1;

    SECTION("No effect if zero")
    {
        JGV instruction({4});
        instruction(registers);

        REQUIRE(registers.pc == 1);
        REQUIRE(registers.status2.membank == 0);
    }

    SECTION("Jump if greater than zero")
    {
        JGV instruction({4});
        registers.val = 2;
        instruction(registers);

        REQUIRE(registers.pc == 0);
        REQUIRE(registers.status2.membank == 4);
    }

    SECTION("No effect if negative")
    {
        JGV instruction({4});
        registers.val = 1;
        registers.status2.negative = true;
        instruction(registers);

        REQUIRE(registers.pc == 1);
        REQUIRE(registers.status2.membank == 0);
    }
}

TEST_CASE("JMP")
{
    Registers registers;

    SECTION("Jump if zero")
    {
        JMP instruction({4});
        instruction(registers);

        REQUIRE(registers.pc == 0);
    }

    SECTION("Jump if greater than zero")
    {
        JMP instruction({4});
        registers.val = 2;
        instruction(registers);

        REQUIRE(registers.pc == 0);
    }

    SECTION("Jump if negative")
    {
        JMP instruction({4});
        registers.val = 1;
        registers.status2.negative = true;
        instruction(registers);

        REQUIRE(registers.pc == 0);
    }
}

TEST_CASE("CAD")
{
    Registers registers;

    SECTION("No sync")
    {
        SECTION("No carry, initially zero")
        {
            CAD instruction({2, 0});
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
        CAD instruction({2, 1});
        instruction(registers);

        REQUIRE(registers.val == 2);
        REQUIRE(!registers.status2.carry);
        REQUIRE(!registers.status2.negative);
        REQUIRE(registers.status1.sync);
    }
}

TEST_CASE("CSU")
{

}

TEST_CASE("LCS")
{
    Registers registers;
    Registers registers_expected;

    SECTION("No sync")
    {
        SECTION("No overflow 1")
        {
            LLS instruction({1, 0});
            registers.val = 0b0101'0101;
            instruction(registers);

            registers_expected.val = 0b1010'1010;
            REQUIRE(registers == registers_expected);
        }

        SECTION("No overflow 2")
        {
            LLS instruction({3, 0});
            registers.val = 0b0001'1101;
            instruction(registers);

            registers_expected.val = 0b1110'1000;
            REQUIRE(registers == registers_expected);
        }

        SECTION("Overflow by 1")
        {
            // The carry flag is updated to the last bit shifted out, bit[n-1], of the register Rm.
            LLS instruction({2, 0});
            registers.val = 0b0101'0101;
            instruction(registers);

            registers_expected.val = 0b0101'0100;
            registers_expected.status2.carry = true;
            REQUIRE(registers == registers_expected);
        }

        SECTION("Overflow by 2")
        {
            LLS instruction({3, 0});
            registers.val = 0b0101'0101;
            instruction(registers);

            registers_expected.val = 0b1010'1000;
            registers_expected.status2.carry = false;
            REQUIRE(registers == registers_expected);
        }

        SECTION("Shift by 8")
        {
            LLS instruction({8, 0});
            registers.val = 0b1111'1111;
            instruction(registers);

            registers_expected.val = 0b0000'0000;
            registers_expected.status2.carry = true;
            REQUIRE(registers == registers_expected);
        }

        REQUIRE(!registers.status1.sync);
    }

    SECTION("Sync")
    {
        LLS instruction({1, 1});
        registers.val = 0b0101'0101;
        instruction(registers);

        registers_expected.val = 0b1010'1010;
        registers_expected.status1.sync = true;
        REQUIRE(registers == registers_expected);
    }
}

TEST_CASE("RLS")
{
    Registers registers;
    Registers registers_expected;

    SECTION("No sync")
    {
        SECTION("No overflow 1")
        {
            RLS instruction({1, 0});
            registers.val = 0b1010'1010;
            instruction(registers);

            registers_expected.val = 0b0101'0101;
            REQUIRE(registers == registers_expected);
        }

        SECTION("No overflow 2")
        {
            RLS instruction({3, 0});
            registers.val = 0b0101'0000;
            instruction(registers);

            registers_expected.val = 0b0000'1010;
            REQUIRE(registers == registers_expected);
        }

        SECTION("Overflow by 1")
        {
            // The carry flag is updated to the last bit shifted out
            RLS instruction({2, 0});
            registers.val = 0b1010'1010;
            instruction(registers);

            registers_expected.val = 0b0010'1010;
            registers_expected.status2.carry = true;
            REQUIRE(registers == registers_expected);
        }

        SECTION("Overflow by 2")
        {
            RLS instruction({3, 0});
            registers.val = 0b1010'1010;
            instruction(registers);

            registers_expected.val = 0b0001'0101;
            registers_expected.status2.carry = false;
            REQUIRE(registers == registers_expected);
        }

        SECTION("Shift by 8")
        {
            RLS instruction({8, 0});
            registers.val = 0b1111'1111;
            instruction(registers);

            registers_expected.val = 0b0000'0000;
            registers_expected.status2.carry = true;
            REQUIRE(registers == registers_expected);
        }

        REQUIRE(!registers.status1.sync);
    }

    SECTION("Sync")
    {
        RLS instruction({1, 1});
        registers.val = 0b1010'1010;
        instruction(registers);

        registers_expected.val = 0b0101'0101;
        registers_expected.status1.sync = true;
        REQUIRE(registers == registers_expected);
    }
}
