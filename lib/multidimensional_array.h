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
    explicit CoreArray(std::vector<size_t> dimensions)
    {
        size = std::accumulate(dimensions.begin(), dimensions.end(), 1ull, std::multiplies<>());
        assert(size <= Tools::umaxof<size_t>() >> 1);

        cores.resize(size);

        for (size_t i = 1; i < dimensions.size(); i++)
        {
            size_t product = 1;

            for (size_t j = i; j < dimensions.size(); j++)
            {
                product *= dimensions.at(j);
            }

            index_operands.push_back(product);
        }

        index_operands.push_back(1);
    }

    explicit CoreArray(std::vector<size_t> dimensions, const Memory_t& mem)
    {
        size = 0;

        for(auto dimension: dimensions)
        {
            for (auto i = 0u; i < dimension; i++, size++)
            {
                cores.emplace_back(*this, size, mem);
            }
        }

        assert(size <= Tools::umaxof<size_t>() >> 1);

        for (auto i = 1u; i < dimensions.size(); i++)
        {
            auto product = 1u;

            for (auto j = i; j < dimensions.size(); j++)
            {
                product *= dimensions.at(j);
            }

            index_operands.push_back(product);
        }

        index_operands.push_back(1);
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"

    Core& operator[](const std::vector<size_t>& index_array)
    {
        assert(index_array.size() == index_operands.size());
        auto index = std::inner_product(index_array.begin(), index_array.end(), index_operands.begin(), size_t{0});
        index = ComputedSettings::WRAP && index >= size ? index - size : index;
        assert(index < size);
        return cores[index];
    }

    const Core& offset(size_t id, const Direction& offsets) const
    {
        assert(ComputedSettings::DIMENSION_N == index_operands.size());
        auto long_size = static_cast<long>(size);
        auto index = std::inner_product(offsets.cbegin(), offsets.cend(), index_operands.begin(), static_cast<long>(id));
        index = ComputedSettings::WRAP && index >= long_size ? index - long_size : index;
        index = ComputedSettings::WRAP && index < 0 ? index + long_size : index;
        assert(index < long_size);
        assert(index >= 0);
        return cores[index];
    }

#pragma clang diagnostic pop

    Core& operator[](const size_t& index)
    {
        return cores[index];
    }

    CoreArray(CoreArray&) = delete;

    CoreArray& operator=(CoreArray&) = delete;

private:
    std::vector<Core> cores;
    std::vector<size_t> index_operands;
    size_t size;
};


#endif //SIMULATOR_MULTIDIMENSIONALARRAY_H
