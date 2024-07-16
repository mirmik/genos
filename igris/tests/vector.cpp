#include <doctest/doctest.h>
#include <igris/container/vector.h>

TEST_CASE("vector")
{
    igris::vector<double> vec;

    vec.emplace_back(15.5);
    vec.emplace_back(15.25);
    vec.emplace_back(3);
    vec.emplace_back(2);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec[1], 15.25);
    CHECK_EQ(vec[2], 3);
    CHECK_EQ(vec[3], 2);

    vec.clear();

    vec.emplace_back(15.5);
    vec.emplace_back(15.25);
    vec.emplace_back(3);
    vec.emplace_back(2);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec[1], 15.25);
    CHECK_EQ(vec[2], 3);
    CHECK_EQ(vec[3], 2);
}

TEST_CASE("vector.push_back")
{
    igris::vector<double> vec;

    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec[1], 15.25);
    CHECK_EQ(vec[2], 3);
    CHECK_EQ(vec[3], 2);

    vec.clear();

    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);
    for (int i = 0; i < 1000; ++i)
        vec.push_back(2);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec[1], 15.25);
    CHECK_EQ(vec[2], 3);
    CHECK_EQ(vec[3], 2);
}

TEST_CASE("vector.pop_back")
{
    igris::vector<double> vec;

    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);

    vec.pop_back();
    vec.pop_back();
    vec.pop_back();
    vec.pop_back();

    CHECK_EQ(vec.size(), 0);
}

TEST_CASE("vector.erase")
{
    igris::vector<double> vec;

    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);

    vec.erase(vec.begin() + 1);
    vec.erase(vec.begin() + 1);
    vec.erase(vec.begin() + 1);
    vec.erase(vec.begin());

    CHECK_EQ(vec.size(), 0);
}

TEST_CASE("vector.insert")
{
    igris::vector<double> vec;

    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);

    vec.insert(vec.begin() + 1, 1);
    vec.insert(vec.begin() + 1, 2);
    vec.insert(vec.begin() + 1, 3);
    vec.insert(vec.begin() + 1, 4);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec[1], 4);
    CHECK_EQ(vec[2], 3);
    CHECK_EQ(vec[3], 2);
    CHECK_EQ(vec[4], 1);
    CHECK_EQ(vec[5], 15.25);
    CHECK_EQ(vec[6], 3);
    CHECK_EQ(vec[7], 2);
}

TEST_CASE("vector.reserve")
{
    igris::vector<double> vec;

    vec.reserve(1000);
    vec.push_back(15.5);
    vec.push_back(15.25);
    vec.push_back(3);
    vec.push_back(2);

    CHECK_EQ(vec[0], 15.5);
    CHECK_EQ(vec.capacity(), 1000);
    CHECK_EQ(vec.size(), 4);
}

TEST_CASE("vector.erase")
{
    igris::vector<double> vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    vec.erase(std::remove_if(
                  vec.begin(), vec.end(), [](int i) { return i % 2 == 0; }),
              vec.end());

    CHECK_EQ(vec[0], 1);
    CHECK_EQ(vec[1], 3);
    CHECK_EQ(vec[2], 5);
    CHECK_EQ(vec[3], 7);
    CHECK_EQ(vec[4], 9);
}