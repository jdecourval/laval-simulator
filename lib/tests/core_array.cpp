#include <memory.h>
#include <core_array.h>
#include <opcodes.h>
#include <direction.h>
#include "catch.hpp"


// TODO: Use operator== to compare cores
TEST_CASE("ID are correctly set and operator[](size_t) return them in the correct order")
{
    Memory_t mem{};
    Registers test_registers;

    CoreArray cores({4, 4, 4}, mem);

    REQUIRE(cores.size() == 4 * 4 * 4);

    auto id = 0u;
    for (auto& core: cores)
    {
        core.execute(DBG{&test_registers});
        REQUIRE(test_registers.id == id);
        id++;
    }
}

TEST_CASE("Operator[](std::vector)")
{
    Memory_t mem{};
    Registers test_registers;

    CoreArray cores({4, 4, 4}, mem);

    auto id = 0u;
    for (auto i = 0ul; i < 4; ++i)
    {
        for (auto j = 0ul; j < 4; ++j)
        {
            for (auto k = 0ul; k < 4; ++k)
            {
                auto& core = cores[{i, j, k}];
                core.execute(DBG{&test_registers});
                REQUIRE(test_registers.id == id);
                REQUIRE(&core == &cores[id]);
                id++;
            }
        }
    }
}

TEST_CASE("Offsets")
{
    Memory_t mem{};
    Registers test_registers;

    CoreArray cores({4, 4, 4}, mem);
    auto& core = cores[{1, 1, 1}];
    core.execute(DBG{&test_registers});

    {
        auto& offsetted = cores.offset(test_registers.id,
            Direction({Direction::CURRENT, Direction::CURRENT, Direction::CURRENT}));
        REQUIRE(core == offsetted);
    }

    {
        auto& offsetted = cores.offset(test_registers.id,
            Direction({Direction::BEFORE, Direction::CURRENT, Direction::CURRENT}));
        auto& compare = cores[{0, 1, 1}];
        REQUIRE(compare == offsetted);
    }

    {
        auto& offsetted = cores.offset(test_registers.id,
            Direction({Direction::AFTER, Direction::AFTER, Direction::AFTER}));
        auto& compare = cores[{2, 2, 2}];
        REQUIRE(compare == offsetted);
    }
}

// TODO: Test at boundaries
