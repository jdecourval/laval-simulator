template<uint8_t... ArgSize>
uint8_t Instruction<ArgSize...>::argument() const
{
//    return *this & (std::numeric_limits<uint8_t>::max() >> ArgSize);
    return 0;
}

template<uint8_t... ArgSize>
void Instruction<ArgSize...>::sync(Registers& registers)
{

}

