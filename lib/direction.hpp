constexpr decltype(auto) Direction::cbegin() const noexcept
{
    return directions.cbegin();
}

constexpr decltype(auto) Direction::begin() const noexcept
{
    return directions.begin();
}

constexpr decltype(auto) Direction::cend() const noexcept
{
    return directions.cend();
}

constexpr decltype(auto) Direction::end() const noexcept
{
    return directions.end();
}

constexpr size_t Direction::total()
{
    // 3 come from the number of possible direction: CURRENT, BEFORE, AFTER
    return static_cast<size_t>(std::pow(3, ComputedSettings::DIMENSION_N));
}
