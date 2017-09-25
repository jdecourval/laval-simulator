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

