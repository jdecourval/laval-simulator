#ifndef SIMULATOR_TOOLS_H
#define SIMULATOR_TOOLS_H

#include <cstddef>

#define bits_of(c, field) Tools::bitsof(Tools::umaxof<c>().field)


namespace Tools
{

    constexpr unsigned long upper_power_of_two(unsigned long v);

    constexpr size_t log2(size_t n);

    template<class T>
    T umaxof();

    template<class T>
    size_t bitsof(const T& umax);


}

#include "tools.hpp"

#endif //SIMULATOR_TOOLS_H
