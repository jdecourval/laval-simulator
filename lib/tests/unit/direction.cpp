#include <catch.hpp>

#include "direction.h"


TEST_CASE("Direction decode")
{
    SECTION("Always result in the right variant")
    {
        auto size = Direction::total_core_directions();
        INFO("Total directions: " << size);

        auto i = uint8_t{};

        for (; i < size; i++)
        {
            auto direction = Direction::decode(i);

            REQUIRE(std::holds_alternative<Direction::CoreDirection>(direction));
        }

        for (; i < static_cast<uint8_t>(Direction::SpecialDirection::LAST_DO_NOT_USE); i++)
        {
            auto direction = Direction::decode(i);

            REQUIRE(std::holds_alternative<Direction::SpecialDirection>(direction));
        }

        for (; i < 0xffu; i++)
        {
            REQUIRE_THROWS_AS(Direction::decode(i), std::out_of_range);
        }
    }

    SECTION("Test known values")
    {
        REQUIRE(std::get<Direction::CoreDirection>(Direction::decode(0u)) ==
                Direction::CoreDirection({
                        Direction::Direction1D::BEFORE,
                        Direction::Direction1D::BEFORE,
                        Direction::Direction1D::BEFORE
                }));

        REQUIRE(std::get<Direction::CoreDirection>(Direction::decode(3u)) ==
                Direction::CoreDirection({
                        Direction::Direction1D::BEFORE,
                        Direction::Direction1D::CURRENT,
                        Direction::Direction1D::BEFORE
                }));

        REQUIRE(std::get<Direction::CoreDirection>(Direction::decode(14u)) ==
                Direction::CoreDirection({
                        Direction::Direction1D::AFTER,
                        Direction::Direction1D::CURRENT,
                        Direction::Direction1D::CURRENT
                }));

        REQUIRE(std::get<Direction::CoreDirection>(Direction::decode(22u)) ==
                Direction::CoreDirection({
                        Direction::Direction1D::CURRENT,
                        Direction::Direction1D::CURRENT,
                        Direction::Direction1D::AFTER
                }));
    }

    SECTION("Special directions")
    {
        {
            auto direction = Direction::decode(static_cast<uint8_t>(Direction::SpecialDirection::PC));
            REQUIRE(std::get<Direction::SpecialDirection>(direction) == Direction::SpecialDirection::PC);
        }

        {
            auto direction = Direction::decode(static_cast<uint8_t>(Direction::SpecialDirection::MEMBANK));
            REQUIRE(std::get<Direction::SpecialDirection>(direction) == Direction::SpecialDirection::MEMBANK);
        }
    }
}
