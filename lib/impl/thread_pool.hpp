template <size_t Cores>
template<class InputIt, class UnaryFunction>
void ThreadPool<Cores>::apply(InputIt first, InputIt last, UnaryFunction f)
{
    if constexpr (Cores == 1)
    {
        std::for_each(first, last, f);
        return;
    }

    auto total = last - first;

    auto i = 0;
    for (auto& core: cores)
    {
        auto begin = first + total * i / cores.size();
        auto end = first + total * (i + 1) / cores.size();
        core = [begin, end, f]() { std::for_each(begin, end, f); };
        i++;
    }

    for (auto& core: cores)
    {
        core.join();
    }
}

