template<typename T>
void InstructionFactory::register_instruction()
{
    register_helper<T>(typename T::Args{});
}

// The following function totally confuses CLion without this definition
#ifdef __JETBRAINS_IDE__
#define constexpr
#endif

template<typename T, size_t... I>
void InstructionFactory::register_helper(std::index_sequence<I...>)
{
    auto end = counter + 1L;

    if constexpr (sizeof...(I) > 0)
    {
        end = ((1 << I) + ...) +counter;
    }

    assert(end <= 256);

    auto begin = counter;

    for (; counter < end; counter++)
    {
        map.at(counter) = [begin](uint8_t val)
        {
            if constexpr (sizeof...(I) > 0)
            {
                assert(val > begin);
                return std::make_unique<T>(static_cast<std::byte>(val - begin));
            }
            else
            {
                assert(val == begin);
                return std::make_unique<T>();
            }
        };

        dump << static_cast<int>(counter) << ":" << typeid(T).name() << std::endl;
    }
}
