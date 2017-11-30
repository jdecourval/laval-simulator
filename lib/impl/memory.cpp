#include "memory.h"

#include "tools.h"


Memory::Memory(const Settings& settings)
: banks(settings.bank_number, std::vector<uint8_t>(settings.bank_size))
{

}

MemoryInterface::size_type Memory::banks_size() const
{
    return !banks.empty() ? banks.at(0).size() : 0;
}

MemoryInterface::size_type Memory::banks_number() const
{
    return banks.size();
}

gsl::span<uint8_t> Memory::at(MemoryInterface::size_type i)
{
    // TODO: Disable bound checking in release builds
    return gsl::make_span(banks.at(i));
}

gsl::span<const uint8_t> Memory::at(MemoryInterface::size_type i) const
{
    // TODO: Disable bound checking in release builds
    return gsl::make_span(banks.at(i));
}
