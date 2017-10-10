#include "catch.hpp"

#include <core.h>
#include <core_array.h>
#include <opcodes.h>
#include <iostream>


// TODO: Those are the names that appear in the console. Should choose them to include tested class. Or use tag ?
TEST_CASE("Single core tests")
{
    Core core;
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

TEST_CASE("Tests that need a linked core")
{
    Memory_t memory{};
    CoreArray core_array({1, 1, 1}, memory);
    Core& core = core_array[0];
    Registers test_registers;

    core.execute(DBG{&test_registers});

    core.execute(DBG{&test_registers});
    REQUIRE(test_registers.id == 0);

    SECTION("Test ids")
    {

    }

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

TEST_CASE("Tests with memory")
{
    SECTION("Execute from linked memory")
    {
        Memory_t memory{};
        CoreArray core_array({1, 1, 1}, memory);
        Core& core = core_array[{0, 0, 0}];
        Registers test_registers;

        core.execute(DBG{&test_registers});

        REQUIRE(test_registers.pc == 0);
        REQUIRE(test_registers.status2.membank == 0);
        REQUIRE(test_registers.val == 0);

        memory[0][0] = core.get_factory().dump(CAD({1}));
        memory[0][1] = core.get_factory().dump(NOP());

        core.step();
        core.step();

        core.execute(DBG{&test_registers});

        REQUIRE(test_registers.pc == 2);
        REQUIRE(test_registers.status2.membank == 0);
        REQUIRE(test_registers.val == 1);

        SECTION("Overstepping should execute NOP instruction until PC wrap")
        {
            Registers test_registers_after_nop = test_registers;
            test_registers_after_nop.pc++;

            for (; test_registers_after_nop.pc != 0; ++test_registers_after_nop.pc)
            {
                core.step();
                core.execute(DBG{&test_registers});

                REQUIRE(test_registers == test_registers_after_nop);
            }

            core.step();
            core.execute(DBG{&test_registers});
            REQUIRE(test_registers == test_registers_after_nop);

            // PC should have wrap, first instruction supposed to execute again
            REQUIRE(test_registers.pc == 0);
            core.step();
            core.execute(DBG{&test_registers});
            REQUIRE(test_registers.val == 2);

        }
    }

    SECTION("Muxed instruction should stall pipeline if no value have been preloaded")
    {

    }
}


TEST_CASE("Fetch from linked core")
{
    // value + carry
}

