template<uint8_t... ArgSizes>
constexpr Instruction<ArgSizes...>::Instruction()
{
    static_assert(sizeof...(ArgSizes) == 0, "Not Default Constructible");
}

template<uint8_t... ArgSizes>
constexpr Instruction<ArgSizes...>::Instruction(std::array<uint8_t, sizeof...(ArgSizes)> args)
: args(args)
{

}

template<uint8_t... ArgSizes>
constexpr Instruction<ArgSizes...>::Instruction(std::byte args_raw)
{
    static_assert((ArgSizes + ...) <= 8);

    set_arg(args_raw);
}

template<uint8_t... ArgSizes>
constexpr uint8_t Instruction<ArgSizes...>::get_argument(uint8_t arg_index) const
{
    return args.at(arg_index);
}

template<uint8_t... ArgSizes>
constexpr void Instruction<ArgSizes...>::sync(Registers& registers)
{
    registers.status1.sync = true;
}

template<uint8_t... ArgSizes>
constexpr void Instruction<ArgSizes...>::set_arg(const std::byte args_raw)
{
    // Assert argument size
#ifndef NDEBUG
        if ((args_raw & ~bitmask<std::byte>((ArgSizes + ...))) != std::byte{0})
        {
            throw std::out_of_range("Argument out of range");
        }
#endif

    set_arg<ArgSizes...>(args_raw, 0);
}

template<uint8_t... ArgSizes>
template<uint8_t FirstArgSize, uint8_t SecondArgSize, uint8_t... OtherArgSizes>
constexpr void Instruction<ArgSizes...>::set_arg(std::byte args_raw, uint8_t i)
{
    args.at(i) = static_cast<uint8_t>(args_raw & bitmask<std::byte>(FirstArgSize));

    set_arg<SecondArgSize, OtherArgSizes...>(args_raw >> FirstArgSize, static_cast<uint8_t>(i + 1));
}

template<uint8_t... ArgSizes>
template<uint8_t FirstArgSize>
constexpr void Instruction<ArgSizes...>::set_arg(std::byte args_raw, uint8_t i)
{
    assert(i == args.size() - 1);

    args.at(i) = static_cast<uint8_t>(args_raw & bitmask<std::byte>(FirstArgSize));
}

#include <climits>

template<uint8_t... ArgSizes>
template<typename T>
constexpr T Instruction<ArgSizes...>::bitmask(unsigned int const onecount)
{
    return static_cast<T>(-(onecount != 0)) & (static_cast<T>(-1) >> ((sizeof(T) * CHAR_BIT) - onecount));
}

template<uint8_t... ArgSizes>
template<typename T>
constexpr bool Instruction<ArgSizes...>::checkcarry(T val)
{
    return val > std::numeric_limits<decltype(Registers::val)>().max();
}
