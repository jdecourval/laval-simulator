#include "demangle.h"

template<typename T>
void InstructionFactory::register_instruction()
{
    register_helper<T>(typename T::Args{});
}

template<typename T, size_t... I>
void InstructionFactory::register_helper(std::index_sequence<I...>)
{
    auto end = counter;

    if constexpr (sizeof...(I) > 0)
    {
        assert((... && I));  // No argument must have a size of 0
        end += ((I + 1) * ...);
    }
    else
    {
        end++;
    }

    assert(end <= 0xff);

    auto begin = counter;
    instruction_to_offset.emplace(typeid(T).hash_code(), begin);

    for (; counter < end; counter++)
    {
        map.at(counter) = [begin](uint8_t val)
        {
            if constexpr (sizeof...(I) > 0)
            {
                assert(val >= begin);
                return std::make_unique<T>(static_cast<std::byte>(val - begin));
            }
            else
            {
                assert(val == begin);
                return std::make_unique<T>();
            }
        };

        auto name = class_name<T>();
        assert(name.size() >= 3);
        name_to_instruction.emplace(name.substr(name.size() - 3), counter);
    }
}
