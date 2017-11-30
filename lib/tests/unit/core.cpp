#include <catch.hpp>

#include <core.h>

#include "test_instructions.h"

#include "direction.h"
#include "opcodes.h"
#include "impl/core_array.h"


// TODO: Those are the names that appear in the console. Should choose them to include tested class. Or use tag ?
TEST_CASE("Single core tests")
{
    Core core;
    Registers test_registers;

    core.execute(Debug{test_registers});

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

        core.execute(OpCodes::SYN{});
        core.execute(Debug{test_registers});
        REQUIRE(test_registers.status1.sync);
    }

    SECTION("Special direction MEMBANK")
    {
        REQUIRE(test_registers.status2.membank == 0);

        core.execute(OpCodes::JMP({1}));
        core.execute(Debug{test_registers});
        REQUIRE(test_registers.status2.membank == 1);
    }

    SECTION("Special direction VAL")
    {
        REQUIRE(test_registers.val == 0);

        core.execute(OpCodes::LCL({4}));
        core.execute(Debug{test_registers});
        REQUIRE(test_registers.val == 4);
    }
}

TEST_CASE("Tests that need memory")
{
    Memory memory(Settings{{1, 1, 1}, 1, 10});
    CoreArray core_array({1, 1, 1}, memory);
    Core& core = core_array[0];
    Registers test_registers;

    core.execute(Debug{test_registers});

    core.execute(Debug{test_registers});
    REQUIRE(test_registers.id == 0);

    SECTION("Test ids")
    {

    }

    SECTION("PC increments")
    {
        REQUIRE(test_registers.pc == 0);

        for (auto i = 1u; i < memory.banks_size(); i++)
        {
            core.fetch();
            core.execute(Debug{test_registers});
            REQUIRE(test_registers.pc == i);
        }

        core.fetch();
        core.execute(Debug{test_registers});
        REQUIRE(test_registers.pc == 0);
    }

    SECTION("Special direction PC")
    {
        REQUIRE(test_registers.pc == 0);
        REQUIRE(!test_registers.preload);

        auto instruction = OpCodes::MUX({DirectionComplex{SpecialDirection::PC}.dump()});
        core.execute(instruction);
        core.preload();

        core.execute(Debug{test_registers});
        REQUIRE(test_registers.preload.value() == 0);

        // To increment PC
        core.fetch();

        core.preload();

        core.execute(Debug{test_registers});
        REQUIRE(test_registers.preload.value() == 1);
    }
}

TEST_CASE("Tests with memory")
{
    SECTION("Execute from linked memory")
    {
        Memory memory(Settings{{1, 1, 1}, 1, 50});
        CoreArray core_array({1, 1, 1}, memory);
        Core& core = core_array[{0, 0, 0}];
        Registers test_registers;

        core.execute(Debug{test_registers});

        REQUIRE(test_registers.pc == 0);
        REQUIRE(test_registers.status2.membank == 0);
        REQUIRE(test_registers.val == 0);

        memory.at(0)[0] = core.get_factory().dump(OpCodes::CAD({1}));
        memory.at(0)[1] = core.get_factory().dump(OpCodes::NOP());

        core.step();
        core.step();

        core.execute(Debug{test_registers});

        REQUIRE(test_registers.pc == 2);
        REQUIRE(test_registers.status2.membank == 0);
        REQUIRE(test_registers.val == 1);

        SECTION("Overstepping should execute NOP instruction until PC wrap")
        {
            Registers test_registers_after_nop = test_registers;
            test_registers_after_nop.pc++;

            for (; test_registers.pc < memory.banks_size() - 1; ++test_registers_after_nop.pc)
            {
                core.step();
                core.execute(Debug{test_registers});

                REQUIRE(test_registers == test_registers_after_nop);
            }

            // pc should now wrap back to zero
            core.step();
            core.execute(Debug{test_registers});
            test_registers_after_nop.pc = 0;
            REQUIRE(test_registers == test_registers_after_nop);

            // PC should have wrap, first instruction supposed to execute again
            REQUIRE(test_registers.pc == 0);
            core.step();
            core.execute(Debug{test_registers});
            REQUIRE(test_registers.val == 2);

        }
    }

    SECTION("Muxed instruction should stall pipeline if no value have been preloaded")
    {

    }
}


TEST_CASE("Fetch from linked core")
{
    Memory memory(Settings{{1, 1, 1}, 1, 50});
    CoreArray core_array({1, 1, 2}, memory);
    Core& core1 = core_array[{0, 0, 0}];
    Core& core2 = core_array[{0, 0, 1}];
    Registers test_registers;

    // Move one bit into carry
    core2.execute(OpCodes::LCH({8}));
    core2.execute(OpCodes::LLS({1, 0}));

    core2.execute(Debug{test_registers});
    REQUIRE(test_registers.status2.carry);

    // Load a know value
    core2.execute(OpCodes::LCL({2}));

    // Connect core1 to core2
    core1.execute(OpCodes::MUX(Direction({Direction::CURRENT, Direction::CURRENT, Direction::AFTER}).dump()));

    SECTION("Do not spontaneously load if no sync")
    {
        // We have to manually preload since its not automatic from execute
        core1.preload();
        core1.execute(Debug{test_registers});
        REQUIRE(test_registers.val == 0);
    }

    SECTION("Do not load if no sync")
    {
        core1.preload();
        core1.execute(OpCodes::MXL({0}));

        core1.execute(Debug{test_registers});
        REQUIRE(test_registers.val == 0);
    }


    SECTION("Load from val")
    {
        core2.execute(OpCodes::SYN());

        core1.preload();
        core1.execute(OpCodes::MXL({0}));

        core1.execute(Debug{test_registers});
        REQUIRE(test_registers.val == 2);
    }

    SECTION("Load from carry")
    {
        core1.execute(OpCodes::CTC());

        core2.execute(OpCodes::SYN());

        core1.preload();
        core1.execute(OpCodes::MXL({0}));

        core1.execute(Debug{test_registers});
        REQUIRE(test_registers.val == 1);
    }

    // Core2 should never be affected
    core2.execute(Debug{test_registers});
    REQUIRE(test_registers.val == 2);
    REQUIRE(test_registers.status2.carry);
}

