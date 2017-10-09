#ifndef SIMULATOR_SETTINGS_H
#define SIMULATOR_SETTINGS_H

#include <numeric>
#include <vector>

#include "tools.h"


// TODO: Reduce classes reliance on settings
namespace UserSettings
{
    constexpr std::initializer_list<size_t> DIMENSIONS = {10, 10, 10};
    constexpr auto WRAP = false;
};

// TODO: Change to a class to avoid static variable that may throw an exception that cannot be caught
// TODO: Move variable from ComputedSettings to UserSettings
namespace ComputedSettings
{
    using PC_t = uint8_t;

    constexpr auto DIMENSION_N = UserSettings::DIMENSIONS.size();
    inline const auto DIMENSIONS = UserSettings::DIMENSIONS;
    const auto CORES = std::accumulate(UserSettings::DIMENSIONS.begin(), UserSettings::DIMENSIONS.end(), 1, std::multiplies<>());
    constexpr auto WRAP = UserSettings::WRAP;

    constexpr auto BANK_NUMBER = 16;
    constexpr auto BANK_NUMBER_BITS = Tools::log2(Tools::upper_power_of_two(BANK_NUMBER));

    constexpr auto BANK_SIZE = 1u << sizeof(PC_t);


    constexpr auto MUX_BITS = Tools::upper_power_of_two(DIMENSION_N);


};

#endif //SIMULATOR_SETTINGS_H
