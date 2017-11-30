#include <catch.hpp>

#include "impl/instruction_factory.h"


template<uint8_t... ArgSizes>
struct TestInstruction : Instruction<ArgSizes...>
{
    using Instruction<ArgSizes...>::Instruction;

    bool operator()(Registers&) const override
    {
        return true;
    }
};

TEST_CASE("Instruction factory")
{
    InstructionFactory factory;

    using Size2 = TestInstruction<1, 1>;
    using Size0 = TestInstruction<>;
    using Size3 = TestInstruction<3>;

    factory.register_instruction<Size2>();
    factory.register_instruction<Size0>();
    factory.register_instruction<Size3>();

    SECTION("Dump")
    {
        {
            auto size2_raw = factory.dump(Size2(std::byte{0}));
            REQUIRE(size2_raw == 0);
        }

        {
            auto size2_raw = factory.dump(Size2(std::byte{1}));
            REQUIRE(size2_raw == 1);
        }

        {
            auto size0_raw = factory.dump(Size0());
            REQUIRE(size0_raw == 4);
        }

        {
            auto size3_raw = factory.dump(Size3(std::byte{0}));
            REQUIRE(size3_raw == 5);
        }

        {
            auto size3_raw = factory.dump(Size3(std::byte{3}));
            REQUIRE(size3_raw == 8);
        }
    }

    SECTION("Create")
    {
        {
            auto instruction = factory.create(0);
            REQUIRE_NOTHROW(dynamic_cast<Size2&>(*instruction));
        }

        {
            auto instruction = factory.create(1);
            REQUIRE_NOTHROW(dynamic_cast<Size2&>(*instruction));
        }

        {
            auto instruction = factory.create(4);
            REQUIRE_NOTHROW(dynamic_cast<Size0&>(*instruction));
        }

        {
            auto instruction = factory.create(5);
            REQUIRE_NOTHROW(dynamic_cast<Size3&>(*instruction));
        }

        {
            auto instruction = factory.create(8);
            REQUIRE_NOTHROW(dynamic_cast<Size3&>(*instruction));
        }

        {
            auto instruction = factory.create(0);
            REQUIRE_THROWS_AS(dynamic_cast<Size3&>(*instruction), const std::bad_cast&);
        }
    }
}

