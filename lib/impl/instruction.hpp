#include <climits>

#include "tools.h"


template<uint8_t... ArgsMaxes>
constexpr Instruction<ArgsMaxes...>::Instruction()
{
    static_assert(sizeof...(ArgsMaxes) == 0, "Not Default Constructible");
}

template<uint8_t... ArgsMaxes>
constexpr Instruction<ArgsMaxes...>::Instruction(const std::array<uint8_t, sizeof...(ArgsMaxes)>& args)
: args(args)
{

}

template<uint8_t... ArgsMaxes>
constexpr Instruction<ArgsMaxes...>::Instruction(std::byte args_raw)
{
    static_assert((ArgsMaxes + ...) <= 0xff);

    set_arg(std::to_integer<uint8_t>(args_raw));
}

template<uint8_t... ArgsMaxes>
constexpr uint8_t Instruction<ArgsMaxes...>::get_argument(uint8_t arg_index) const
{
    return args.at(arg_index);
}

template<uint8_t... ArgsMaxes>
constexpr void Instruction<ArgsMaxes...>::sync(Registers& registers)
{
    registers.status1.sync = true;
}

template<uint8_t... ArgsMaxes>
constexpr void Instruction<ArgsMaxes...>::set_arg(uint8_t args_raw)
{
    // Check if argument fits in the instruction
#ifndef NDEBUG
    if (args_raw >= ((ArgsMaxes + 1) * ...))
    {
        throw std::out_of_range("Too big arguments");
    }
#endif

    set_arg<ArgsMaxes...>(args_raw, 0);
}

template<uint8_t... ArgsMaxes>
template<uint8_t FirstArgMax, uint8_t SecondArgSize, uint8_t... OtherArgsMaxes>
constexpr void Instruction<ArgsMaxes...>::set_arg(uint8_t args_raw, uint8_t i)
{
    set_arg<FirstArgMax>(args_raw, i);

    set_arg<SecondArgSize, OtherArgsMaxes...>(args_raw / (FirstArgMax + 1_u8), static_cast<uint8_t>(i + 1));
}

template<uint8_t... ArgsMaxes>
template<uint8_t FirstArgMax>
constexpr void Instruction<ArgsMaxes...>::set_arg(uint8_t args_raw, uint8_t i)
{
    args.at(i) = args_raw % (FirstArgMax + 1_u8);
}

template<uint8_t... ArgsMaxes>
template<typename T>
constexpr bool Instruction<ArgsMaxes...>::checkcarry(T val)
{
    return val > std::numeric_limits<decltype(Registers::val)>().max();
}

template<uint8_t... ArgsMaxes>
uint8_t Instruction<ArgsMaxes...>::dump_args() const
{
    if constexpr (sizeof...(ArgsMaxes) != 0)
    {
        return dump_args<ArgsMaxes...>(0, 1);
    }

    return 0;
}

template<uint8_t... ArgsMaxes>
template<uint8_t FirstArgMax, uint8_t SecondArgSize, uint8_t... OtherArgsMaxes>
uint8_t Instruction<ArgsMaxes...>::dump_args(uint8_t i, uint8_t accumulated_max) const
{
    return dump_args<FirstArgMax>(i, accumulated_max ) + dump_args<SecondArgSize, OtherArgsMaxes...>(i + 1_u8, accumulated_max * (FirstArgMax + 1));
}

// TODO: Natural size integers may be faster where having an uint8_t is not meaningful. Benchmark first.
template<uint8_t... ArgsMaxes>
template<uint8_t LastArgSize>
uint8_t Instruction<ArgsMaxes...>::dump_args(uint8_t i, uint8_t accumulated_max) const
{
    return args.at(i) * accumulated_max;
}

template<uint8_t... ArgsMaxes>
void Instruction<ArgsMaxes...>::load_args(const std::vector<uint8_t>& args)
{
    assert(args.size() == sizeof...(ArgsMaxes));

    std::copy(std::cbegin(args), std::cend(args), std::begin(this->args));
}

