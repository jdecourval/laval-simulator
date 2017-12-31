#ifndef SIMULATOR_MULTIDIMENSIONALARRAY_H
#define SIMULATOR_MULTIDIMENSIONALARRAY_H

#include "core.h"
#include "direction.h"


class CoreArray
{
    static constexpr auto Wrap = false;

public:
    explicit CoreArray(const std::vector<uint16_t>& dimensions, const MemoryInterface& mem);

    Core& operator[](const size_t& index);

    Core& operator[](const std::array<size_t, 3>& index_array);

    Core& offset(size_t id, const Direction::CoreDirection& offsets);

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
