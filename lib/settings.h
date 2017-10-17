#ifndef SIMULATOR_SETTINGS_H
#define SIMULATOR_SETTINGS_H

#include <numeric>
#include <vector>

#include "tools.h"


// TODO: Reduce classes reliance on settings
// TODO: This should be moved to source folder
namespace UserSettings
{
    constexpr std::initializer_list<size_t> DIMENSIONS = {10, 10, 10};
    constexpr auto BANK_NUMBER = 16;
    constexpr auto BANK_SIZE = 256;
};

// TODO: Change to a class to avoid static variable that may throw an exception that cannot be caught

#endif //SIMULATOR_SETTINGS_H
