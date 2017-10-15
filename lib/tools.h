#ifndef SIMULATOR_TOOLS_H
#define SIMULATOR_TOOLS_H

#include <cstddef>
#include <cassert>


constexpr uint8_t operator "" _u8(unsigned long long value)
{
    return static_cast<uint8_t>(value);
}

/// Perform the fastest shift possible while ensuring the undefined behaviour may happen
constexpr uint8_t safe_right_shift(uint8_t a, unsigned long b)
{
    // Right shifting is safe as long as:
    // a and b are positive which is guaranteed for unsigned numbers
    // b is smaller than the number of bits in the promoted a
    // Although this function is more severe since there really is no point in shifting more than the real integer size
    assert(b < sizeof(a) * 8);

    return static_cast<uint8_t>(a >> b);
}

/// Perform the fastest shift possible while ensuring the undefined behaviour may happen
constexpr uint8_t safe_left_shift(uint8_t a, unsigned long b)
{
    // Right shifting is safe as long as:
    // a and b are positive which is guaranteed for unsigned numbers
    // b is smaller than the number of bits in the promoted a
    // Although this function is more severe since there really is no point in shifting more than the real integer size
    assert(b < sizeof(a) * 8);

    return static_cast<uint8_t>(a << b);
}

namespace Tools
{
    constexpr unsigned long upper_power_of_two(unsigned long v);

    constexpr size_t log2(size_t n);

    template<class T>
    T umaxof();
}

#include "tools.hpp"

#endif //SIMULATOR_TOOLS_H
