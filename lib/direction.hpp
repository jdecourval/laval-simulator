constexpr decltype(auto) Direction::cbegin() const noexcept
{
    return directions.cbegin();
}

constexpr decltype(auto) Direction::begin() noexcept
{
    return directions.begin();
}

constexpr decltype(auto) Direction::cend() const noexcept
{
    return directions.cend();
}

constexpr decltype(auto) Direction::end() noexcept
{
    return directions.end();
}

constexpr size_t Direction::total()
{
    // 3 come from the number of possible direction: CURRENT, BEFORE, AFTER
    return three_pow(ComputedSettings::DIMENSION_N);
}

constexpr uint8_t Direction::three_pow(unsigned long n)
{
    return std::array{1_u8, 3_u8, 9_u8, 27_u8, 81_u8, 243_u8}.at(n);
}
