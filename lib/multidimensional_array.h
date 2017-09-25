#ifndef SIMULATOR_MULTIDIMENSIONALARRAY_H
#define SIMULATOR_MULTIDIMENSIONALARRAY_H

#include <memory>
#include <numeric>
#include <cassert>

#include "settings.h"
#include "core.h"
#include "direction.h"


class CoreArray
{
public:
    CoreArray(std::vector<size_t> dimensions)
    {
        size = std::accumulate(dimensions.begin(), dimensions.end(), 0ull, std::multiplies<size_t>());
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
        assert(offsets.size() == index_operands.size());
        auto index = std::inner_product(offsets.begin(), offsets.end(), index_operands.begin(), static_cast<long>(id));
        index = Settings::WRAP && index >= size ? index - size : index;
        index = Settings::WRAP && index < 0 ? index + size : index;
        assert(index < size);
        assert(index >= 0);
        return cores[index];
    }

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
