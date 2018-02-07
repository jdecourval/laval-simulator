#ifndef SIMULATOR_MULTIDIMENSIONALARRAY_H
#define SIMULATOR_MULTIDIMENSIONALARRAY_H

#include "core.h"
#include "direction.h"
#include "input.h"

#include <map>


class CoreArray
{
    static constexpr auto Wrap = false;
    static inline std::map<size_t, Input> default_input;

public:
    explicit CoreArray(const std::vector<uint16_t>& dimensions, const MemoryInterface& mem);

    explicit CoreArray(const std::vector<uint16_t>& dimensions, const MemoryInterface& mem, std::map<size_t, Input>& inputs);

    Core& operator[](const size_t& index);

    Core& operator[](const std::array<size_t, 3>& index_array);

    Fetchable& offset(size_t id, const Direction::CoreDirection& offsets);

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
    std::map<size_t, Input>& inputs;
};


#endif //SIMULATOR_MULTIDIMENSIONALARRAY_H
