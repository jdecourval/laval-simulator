#ifndef SIMULATOR_MULTIDIMENSIONALARRAY_H
#define SIMULATOR_MULTIDIMENSIONALARRAY_H

#include <cassert>
#include <memory>
#include <numeric>

#include "core.h"
#include "direction.h"
#include "settings.h"


class CoreArray
{
public:
    explicit CoreArray(std::vector<size_t> dimensions, const Memory_t& mem);

    Core& operator[](const size_t& index);

    Core& operator[](const std::vector<size_t>& index_array);

    const Core& offset(size_t id, const Direction& offsets) const;

    decltype(auto) cbegin() const noexcept
    {
        return std::cbegin(cores);
    }

    decltype(auto) begin() noexcept
    {
        return std::begin(cores);
    }

    decltype(auto) cend() const noexcept
    {
        return std::cend(cores);
    }

    decltype(auto) end() noexcept
    {
        return std::end(cores);
    }

    CoreArray(CoreArray&) = delete;

    CoreArray& operator=(CoreArray&) = delete;

private:
    std::vector<Core> cores;
    std::vector<size_t> index_operands;
};


#endif //SIMULATOR_MULTIDIMENSIONALARRAY_H
