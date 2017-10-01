template<uint8_t... ArgSizes>
Instruction<ArgSizes...>::Instruction()
{
    static_assert(sizeof...(ArgSizes) == 0, "Not Default Constructible");
}

template<uint8_t... ArgSizes>
Instruction<ArgSizes...>::Instruction(const std::initializer_list<uint8_t> args)
{
    assert(args.size() == sizeof...(ArgSizes));

    auto i = 0u;

    // TODO: There must be a way to statically initialize the array
    for (auto& arg: args)
    {
        this->args.at(i) = arg;
        i++;
    }
}

template<uint8_t... ArgSizes>
Instruction<ArgSizes...>::Instruction(uint8_t args_raw)
{
    assert((ArgSizes + ...) <= 8);

    set_arg(args_raw);
}

template<uint8_t... ArgSizes>
uint8_t Instruction<ArgSizes...>::get_argument(uint8_t arg_index) const
{
    return args.at(arg_index);
}

template<uint8_t... ArgSizes>
void Instruction<ArgSizes...>::sync(Registers& registers)
{
    registers.status1.sync = true;
}

template<uint8_t... ArgSizes>
void Instruction<ArgSizes...>::set_arg(uint8_t args_raw)
{
    // TODO: Convert assert to exceptions
    // Assert argument size
    assert(args_raw <= bitmask<uint8_t>((ArgSizes + ...)));

    set_arg < ArgSizes...>(args_raw, 0);
}

template<uint8_t... ArgSizes>
template<uint8_t FirstArgSize, uint8_t SecondArgSize, uint8_t... OtherArgSizes>
void Instruction<ArgSizes...>::set_arg(uint8_t args_raw, uint8_t i)
{
    assert(FirstArgSize > 0);

    args.at(i) = args_raw & bitmask<uint8_t>(FirstArgSize) << (SecondArgSize + (OtherArgSizes + ... + 0));

    set_arg < SecondArgSize, OtherArgSizes...>(args_raw, static_cast<uint8_t>(i + 1));
}

template<uint8_t... ArgSizes>
template<uint8_t FirstArgSize>
void Instruction<ArgSizes...>::set_arg(uint8_t args_raw, uint8_t i)
{
    assert(FirstArgSize > 0);
    assert(i == args.size() - 1);

    args.at(i) = args_raw & bitmask<uint8_t>(FirstArgSize);
}

#include <climits>

template<uint8_t... ArgSizes>
template<typename T>
constexpr T Instruction<ArgSizes...>::bitmask(unsigned int const onecount)
{
    return static_cast<T>(-(onecount != 0))
           & (static_cast<T>(-1) >> ((sizeof(T) * CHAR_BIT) - onecount));
}

template<uint8_t... ArgSizes>
template<typename T>
constexpr bool Instruction<ArgSizes...>::checkcarry(T val)
{
    return val > std::numeric_limits<decltype(Registers::val)>().max();
}
