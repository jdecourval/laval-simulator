#include "core_array.h"


CoreArray::CoreArray(const std::vector<uint16_t>& dimensions, const MemoryInterface& mem)
: CoreArray(dimensions, mem, default_input)
{

}

CoreArray::CoreArray(const std::vector<uint16_t>& dimensions, const MemoryInterface& mem, std::map<size_t, Input>& inputs)
: inputs(inputs)
{
    auto size = std::accumulate(dimensions.begin(), dimensions.end(), 1ull, std::multiplies<>());
    cpu_assert(size <= Tools::umaxof<size_t>() >> 1, "Too many cores");

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

Core& CoreArray::operator[](const std::array<size_t, 3>& index_array)
{
    auto index = std::inner_product(index_array.begin(), index_array.end(), index_operands.begin(), size_t{0});
    index = Wrap && index >= cores.size() ? index - cores.size() : index;
    cpu_assert(index < cores.size(), "No such core at index " << index_array.at(0) << ":" << index_array.at(1) << ":" << index_array.at(2));
    return cores[index];
}

Fetchable& CoreArray::offset(size_t id, const Direction::CoreDirection& offsets)
{
    cpu_assert(offsets.size() == index_operands.size(), "Offset do not have the right number of dimensions");

    auto long_size = static_cast<long>(cores.size());
    auto index = std::inner_product(offsets.cbegin(), offsets.cend(), index_operands.begin(), static_cast<long>(id),
            std::plus<>(),
            [](auto &offset, auto &a) {
                return a * (static_cast<int>(offset) - 1);
            });

    index = Wrap && index >= long_size ? index - long_size : index;
    index = Wrap && index < 0 ? index + long_size : index;

    if (index < long_size && index >= 0)
    {
        return cores[index];
    }
    else if (auto it = inputs.find(id); it != std::cend(inputs))
    {
        return inputs.at(id);
    }
    else
    {
        cpu_assert(false, "No such core at offset " << offsets.at(0) - 1 << ":" << offsets.at(1) - 1 << ":" << offsets.at(2) - 1);
    }

    return cores[index];
}

Core& CoreArray::operator[](const size_t& index)
{
    return cores[index];
}
