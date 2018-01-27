#include "memory.h"

#include "throw_assert.h"
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
    cpu_assert(i < banks.size(), "Memory access out of bound");
    return gsl::make_span(banks[i]);
}

gsl::span<const uint8_t> Memory::at(MemoryInterface::size_type i) const
{
    cpu_assert(i < banks.size(), "Memory access out of bound");
    return gsl::make_span(banks[i]);
}
