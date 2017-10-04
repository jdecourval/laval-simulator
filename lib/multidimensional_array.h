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
    // TODO: Use an std::initializer_list
    explicit CoreArray(std::vector<size_t> dimensions)
    {
        size = std::accumulate(dimensions.begin(), dimensions.end(), 0ull, std::multiplies<>());
        assert(size <= Tools::umaxof<size_t>() >> 1);

        cores = std::make_unique<Core[]>(size);

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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCSimplifyInspection"

    Core& operator[](const std::vector<size_t>& index_array)
    {
        assert(index_array.size() == index_operands.size());
        auto index = std::inner_product(index_array.begin(), index_array.end(), index_operands.begin(), size_t{0});
        index = Settings::WRAP && index >= size ? index - size : index;
        assert(index < size);
        return cores[index];
    }

    Core& offset(size_t id, const Direction& offsets)
    {
        assert(Settings::DIMENSION_N == index_operands.size());
        auto long_size = static_cast<long>(size);
        auto index = std::inner_product(offsets.cbegin(), offsets.cend(), index_operands.begin(), static_cast<long>(id));
        index = Settings::WRAP && index >= long_size ? index - long_size : index;
        index = Settings::WRAP && index < 0 ? index + long_size : index;
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
    std::unique_ptr<Core[]> cores;
    std::vector<size_t> index_operands;
    size_t size;
};


#endif //SIMULATOR_MULTIDIMENSIONALARRAY_H
