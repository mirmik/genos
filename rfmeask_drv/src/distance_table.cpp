#include <distance_table.h>

void distance_table::clear()
{
    table.clear();
    pointer = 0;
}

int64_t &distance_table::operator[](size_t index)
{
    if (index >= table.size())
        return table[table.size() - 1];
    return table[index];
}

const int64_t &distance_table::operator[](size_t index) const
{
    if (index >= table.size())
        return table[table.size() - 1];
    return table[index];
}

size_t distance_table::size() const
{
    return table.size();
}

void distance_table::fill(int64_t value)
{
    std::fill(table.begin(), table.end(), value);
}

void distance_table::reset_pointer()
{
    pointer = 0;
}

int64_t distance_table::get_next_point()
{
    return table[pointer++];
}

size_t distance_table::avail()
{
    return table.size() - pointer;
}

void distance_table::add_point(int64_t value)
{
    table.push_back(value);
}
