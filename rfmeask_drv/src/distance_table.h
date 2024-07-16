#ifndef DISTANCE_TABLE_H
#define DISTANCE_TABLE_H

#include <cstdint>
#include <cstdlib>
#include <vector>

class distance_table
{
    std::vector<int64_t> table = {};
    int pointer = 0;

public:
    distance_table() = default;
    ~distance_table() = default;

    void clear();
    int64_t &operator[](size_t index);
    const int64_t &operator[](size_t index) const;
    size_t size() const;
    void fill(int64_t value);
    void reset_pointer();
    int64_t get_next_point();
    size_t avail();
    void add_point(int64_t value);
};

#endif