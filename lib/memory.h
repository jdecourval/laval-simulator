#ifndef SIMULATOR_MEMORY_H
#define SIMULATOR_MEMORY_H

#include <array>

#include "settings.h"

using Memory_t = std::array<std::array<uint8_t, ComputedSettings::BANK_SIZE>, ComputedSettings::BANK_NUMBER>;

#endif //SIMULATOR_MEMORY_H
