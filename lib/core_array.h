#ifndef SIMULATOR_MULTIDIMENSIONALARRAY_H
#define SIMULATOR_MULTIDIMENSIONALARRAY_H

#include "core.h"


class Direction;

class CoreArray
{
    static constexpr auto Wrap = false;

public:
    explicit CoreArray(const std::vector<size_t>& dimensions, const MemoryInterface& mem);

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

    decltype(auto) size() noexcept
    {
        return cores.size();
    }

    CoreArray(CoreArray&) = delete;

    CoreArray& operator=(CoreArray&) = delete;

private:
    std::vector<Core> cores;
    std::vector<size_t> index_operands;
};


#endif //SIMULATOR_MULTIDIMENSIONALARRAY_H
