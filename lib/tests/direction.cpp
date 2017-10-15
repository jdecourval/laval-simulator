#include "catch.hpp"

#include "direction.h"


TEST_CASE("Load and dump")
{
    auto size = Direction::total();
    INFO("Total directions: " << size);

    for (uint8_t i = 0; i < size; i++)
    {
        DirectionComplex direction_complex{i};
        Direction direction{i};

        REQUIRE(std::holds_alternative<Direction>(direction_complex));
        REQUIRE(direction.dump() == i);
        REQUIRE(direction_complex.dump() == i);
    }
}

TEST_CASE("Direction constructor")
{
    Direction direction({Direction::AFTER, Direction::CURRENT, Direction::BEFORE});
    auto iter = std::begin(direction);

    REQUIRE(*iter == Direction::AFTER);
    REQUIRE(*(++iter) == Direction::CURRENT);
    REQUIRE(*(++iter) == Direction::BEFORE);
}

TEST_CASE("Special directions")
{
    auto size = Direction::total();

    SECTION("PC")
    {
        DirectionComplex direction{SpecialDirection::PC};
        REQUIRE(std::holds_alternative<SpecialDirection>(direction));
        REQUIRE(direction.dump() == size);
    }

    SECTION("MEMBANK")
    {
        DirectionComplex direction{SpecialDirection::MEMBANK};
        REQUIRE(std::holds_alternative<SpecialDirection>(direction));
        REQUIRE(direction.dump() == size + 1);
    }

    SECTION("PC from raw")
    {
        DirectionComplex direction{static_cast<uint8_t>(size)};
        REQUIRE(std::holds_alternative<SpecialDirection>(direction));
        REQUIRE(direction.dump() == size);
    }

    SECTION("MEMBANK from raw")
    {
        DirectionComplex direction{static_cast<uint8_t>(size + 1)};
        REQUIRE(std::holds_alternative<SpecialDirection>(direction));
        REQUIRE(direction.dump() == size + 1);
    }

    SECTION("Invalid directions")
    {
        REQUIRE_THROWS_AS(Direction{Direction::total()}, const std::out_of_range&);

        REQUIRE_THROWS_AS(DirectionComplex {255}, const std::out_of_range&);

        REQUIRE_THROWS_AS(DirectionComplex {Direction::total() + static_cast<int>(SpecialDirection::LAST_DO_NOT_USE)},
                const std::out_of_range&);

        REQUIRE_THROWS_AS(DirectionComplex {SpecialDirection::LAST_DO_NOT_USE}, const std::out_of_range&);
    }
}
