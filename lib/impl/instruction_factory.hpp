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
        end += 1 << (I + ...);
    }
    else
    {
        end++;
    }

    assert(end <= 256);

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

        dump_file << static_cast<int>(counter) << ":" << typeid(T).name() << std::endl;
    }
}

template<typename T>
uint8_t InstructionFactory::dump(const T& instruction) const
{
    return instruction_to_offset.at(typeid(T).hash_code()) + instruction.dump_args();
}
