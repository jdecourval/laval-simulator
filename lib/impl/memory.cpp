#include "memory.h"

#include "tools.h"


Memory::Memory(unsigned BankNumber, unsigned BankSize)
: banks(BankNumber, std::vector<uint8_t>(BankSize))
{

}

MemoryInterface::size_type Memory::banks_size() const
{
    return banks.size() ? banks.at(0).size() : 0;
}

MemoryInterface::size_type Memory::banks_number() const
{
    return banks.size();
}

gsl::span<uint8_t> Memory::at(MemoryInterface::size_type i)
{
    return gsl::make_span(banks.at(i));
}

gsl::span<const uint8_t> Memory::at(MemoryInterface::size_type i) const
{
    return gsl::make_span(banks.at(i));
}
