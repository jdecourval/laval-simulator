#include "catch.hpp"

#include <core.h>
#include <multidimensional_array.h>
#include <opcodes.h>


// TODO: Those are the names that appear in the console. Should choose them to include tested class. Or use tag ?
TEST_CASE("Single core tests")
{
    CoreArray core_array({1, 1, 1});
    Core& core = core_array[0];
    Registers test_registers;

    core.execute(DBG{&test_registers});

    SECTION("Initialization")
    {
        REQUIRE(test_registers.id == 0);
        REQUIRE(!test_registers.preload);
        REQUIRE(test_registers.pc == 0);
        REQUIRE(test_registers.val == 0);
        REQUIRE(!test_registers.preload_negative);
        REQUIRE(test_registers.status1 == Registers::Status1{});
        REQUIRE(test_registers.status2 == Registers::Status2{});
    }

    SECTION("Tests that need a linked core")
    {
        Memory_t memory{};
        core.link(&core_array, 1, &memory);
        core.execute(DBG{&test_registers});
        REQUIRE(test_registers.id == 1);

        core.link(&core_array, 200, &memory);
        core.execute(DBG{&test_registers});
        REQUIRE(test_registers.id == 200);


        SECTION("PC increments")
        {
            REQUIRE(test_registers.pc == 0);

            for (auto i = 1u; i < 256; i++)
            {
                core.fetch();
                core.execute(DBG{&test_registers});
                REQUIRE(test_registers.pc == i);
            }

            core.fetch();
            core.execute(DBG{&test_registers});
            REQUIRE(test_registers.pc == 0);
        }

        SECTION("Special direction PC")
        {
            REQUIRE(test_registers.pc == 0);
            REQUIRE(!test_registers.preload);

            auto instruction = MUX({DirectionComplex{SpecialDirection::PC}.dump()});
            core.execute(instruction);
            core.preload();

            core.execute(DBG{&test_registers});
            REQUIRE(test_registers.preload.value() == 0);

            // To increment PC
            core.fetch();

            core.preload();

            core.execute(DBG{&test_registers});
            REQUIRE(test_registers.preload.value() == 1);
        }
    }

    SECTION("Cannot fetch if memory is unlinked")
    {
        REQUIRE_THROWS_AS(core.fetch(), const std::logic_error&);
    }

    SECTION("Sync")
    {
        REQUIRE_FALSE(test_registers.status1.sync);

        core.sync();
        core.execute(DBG{&test_registers});
        REQUIRE(test_registers.status1.sync);
    }

    SECTION("Special direction MEMBANK")
    {
        REQUIRE(test_registers.status2.membank == 0);

        core.execute(JMP({1}));
        core.execute(DBG{&test_registers});
        REQUIRE(test_registers.status2.membank == 1);
    }

    SECTION("Special direction VAL")
    {
        REQUIRE(test_registers.val == 0);

        core.execute(LCL({4}));
        core.execute(DBG{&test_registers});
        REQUIRE(test_registers.val == 4);
    }
}

TEST_CASE("Tests with memory")
{
    SECTION("Execute from linked memory")
    {
        CoreArray core_array({4, 4, 4});
        Core& core = core_array[{1, 1, 1}];
        Registers test_registers;

        core.execute(DBG{&test_registers});

        Memory_t memory{};
        core.link(&core_array, 1, &memory);

    }

    SECTION("Muxed instruction should stall pipeline if no value have been preloaded")
    {

    }
}


TEST_CASE("Fetch from linked core")
{
    // value + carry
}

TEST_CASE("New safer construction method")
{
    Memory_t mem{};
    CoreArray core_array({10, 10}, mem);
}

