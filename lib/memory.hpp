template<unsigned BankNumber, unsigned BankSize>
constexpr MemoryInterface::size_type Memory<BankNumber, BankSize>::banks_size() const
{
    return BankSize;
}

template<unsigned BankNumber, unsigned BankSize>
constexpr MemoryInterface::size_type Memory<BankNumber, BankSize>::banks_number() const
{
    return BankNumber;
}

template<unsigned BankNumber, unsigned BankSize>
gsl::span<uint8_t> Memory<BankNumber, BankSize>::at(MemoryInterface::size_type i)
{
    return gsl::make_span(banks.at(i));
}

template<unsigned BankNumber, unsigned BankSize>
gsl::span<const uint8_t> Memory<BankNumber, BankSize>::at(MemoryInterface::size_type i) const
{
    return gsl::make_span(banks.at(i));
}
