#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include <array>
#include <functional>
#include <gsl/gsl>


class MemoryInterface
{
public:
    using size_type = size_t;

    virtual size_type banks_size() const = 0;

    virtual size_type banks_number() const = 0;

    virtual gsl::span<uint8_t> at(size_type i) = 0;

    virtual gsl::span<const uint8_t> at(size_type i) const = 0;
};


template<unsigned BankNumber, unsigned BankSize>
class Memory final : public MemoryInterface
{
public:
    constexpr size_type banks_size() const override final;

    constexpr size_type banks_number() const override final;

    gsl::span<uint8_t> at(size_type i) override final;

    gsl::span<const uint8_t> at(size_type i) const override final;

private:
    std::array<std::array<uint8_t, BankSize>, BankNumber> banks;
};


#include "memory.hpp"

#endif //SIMULATOR_MEMORY_H
