#ifndef SIMULATOR_TOOLS_H
#define SIMULATOR_TOOLS_H

#include <cstddef>


namespace Tools
{
    constexpr unsigned long upper_power_of_two(unsigned long v);

    constexpr size_t log2(size_t n);

    template<class T>
    T umaxof();
}

#include "tools.hpp"

#endif //SIMULATOR_TOOLS_H
