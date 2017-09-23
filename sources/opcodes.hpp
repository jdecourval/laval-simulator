template<uint8_t Op, uint8_t ArgSize>
Instruction<Op, ArgSize>::Instruction(uint8_t val)
        : std::bitset<8>(val)
{
}

template<uint8_t Op, uint8_t ArgSize>
constexpr uint8_t Instruction<Op, ArgSize>::mask()
{
    return std::numeric_limits<uint8_t>::max() >> ArgSize << ArgSize;
}

template<uint8_t Op, uint8_t ArgSize>
uint8_t Instruction<Op, ArgSize>::opcode()
{
    return Op;
}

template<uint8_t Op, uint8_t ArgSize>
uint8_t Instruction<Op, ArgSize>::argument()
{
    return *this & (std::numeric_limits<uint8_t>::max() >> ArgSize);
}

template<uint8_t Op, uint8_t ArgSize>
bool Instruction<Op, ArgSize>::validate(uint8_t arg)
{
    return (arg & mask()) == opcode();
}

template<uint8_t Op, uint8_t ArgSize>
uint8_t Instruction<Op, ArgSize>::operator&(uint8_t other) const
{
    return opcode() & other;
}

template<uint8_t Op, uint8_t ArgSize>
uint8_t Instruction<Op, ArgSize>::operator^(uint8_t other) const
{
    return opcode() ^ other;
}

