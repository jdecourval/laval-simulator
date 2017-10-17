#include <numeric>
#include "core_array.h"

#include "direction.h"


CoreArray::CoreArray(const std::vector<size_t>& dimensions, const MemoryInterface& mem)
{
    auto size = std::accumulate(dimensions.begin(), dimensions.end(), 1ull, std::multiplies<>());
    assert(size <= Tools::umaxof<size_t>() >> 1);

    for (auto id = 0u; id < size; ++id)
    {
        cores.emplace_back(*this, id, mem);
    }

    for (auto i = 1u; i < dimensions.size(); i++)
    {
        auto product = 1ul;

        for (auto j = i; j < dimensions.size(); j++)
        {
            product *= dimensions.at(j);
        }

        index_operands.push_back(product);
    }

    index_operands.push_back(1);
}

Core& CoreArray::operator[](const std::vector<size_t>& index_array)
{
    assert(index_array.size() == index_operands.size());
    auto index = std::inner_product(index_array.begin(), index_array.end(), index_operands.begin(), size_t{0});
    index = Wrap && index >= cores.size() ? index - cores.size() : index;
    assert(index < cores.size());
    return cores[index];
}

const Core& CoreArray::offset(size_t id, const Direction& offsets) const
{
    assert(offsets.size() == index_operands.size());
    auto long_size = static_cast<long>(cores.size());
    auto index = std::inner_product(offsets.cbegin(), offsets.cend(), index_operands.begin(), static_cast<long>(id));
    index = Wrap && index >= long_size ? index - long_size : index;
    index = Wrap && index < 0 ? index + long_size : index;
    assert(index < long_size);
    assert(index >= 0);
    return cores[index];
}

Core& CoreArray::operator[](const size_t& index)
{
    return cores[index];
}
