#include "core.h"
#include "catch.hpp"

TEST_CASE("NOP")
{
    Core core;
    NOP instruction;

    core.execute(instruction);
}
