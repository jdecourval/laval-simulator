#ifndef SIMULATOR_TOOLS_H
#define SIMULATOR_TOOLS_H

#include <cstddef>
#include <cstdint>
#include <cassert>


constexpr uint8_t operator "" _u8(unsigned long long value)
{
    return static_cast<uint8_t>(value);
}

namespace Tools
{
    constexpr unsigned long upper_power_of_two(unsigned long v);

    constexpr size_t log2(size_t n);

    template<class T>
    T umaxof();

    constexpr uint8_t three_pow(unsigned long n);
}

#include "tools.hpp"

#endif //SIMULATOR_TOOLS_H
