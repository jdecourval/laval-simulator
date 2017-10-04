#include "catch.hpp"

#include "direction.h"


TEST_CASE("Load and dump")
{
    auto size = Direction::total();
    INFO("Total directions: " << size);

    for (uint8_t i=0; i<size; i++)
    {
        DirectionComplex direction_complex{i};
        Direction direction{i};

        REQUIRE(std::holds_alternative<Direction>(direction_complex));
        REQUIRE(direction.dump() == i);
        REQUIRE(direction_complex.dump() == i);
    }
}

TEST_CASE("Special directions")
{
    auto size = Direction::total();

    SECTION("PC")
    {
        DirectionComplex pc{static_cast<uint8_t>(size)};
        REQUIRE(std::holds_alternative<SpecialDirection>(pc));
        REQUIRE(pc.dump() == size);
    }

    SECTION("MEMBANK")
    {
        DirectionComplex membank{static_cast<uint8_t>(size + 1)};
        REQUIRE(std::holds_alternative<SpecialDirection>(membank));
        REQUIRE(membank.dump() == size + 1);
    }
}
