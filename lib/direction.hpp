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
