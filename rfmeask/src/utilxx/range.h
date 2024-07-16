#include <ranges>

template <std::ranges::range R> constexpr auto to_vector(R &&r)
{
    using elem_t = std::decay_t<std::ranges::range_value_t<R>>;
    return std::vector<elem_t>{r.begin(), r.end()};
}

template <class Elem> constexpr auto to_vector_type(std::ranges::range auto &&r)
{
    return std::vector<Elem>{r.begin(), r.end()};
}