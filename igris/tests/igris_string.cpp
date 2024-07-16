#include <doctest/doctest.h>
#include <igris/container/std_portable.h>

TEST_CASE("string10")
{
    igris::string str("hello lalala jjj");
}

TEST_CASE("string2")
{
    igris::string str("hello lalala jjj");
    auto spl = str.split(' ');
    CHECK_EQ(spl[0], "hello");
    CHECK_EQ(spl[1], "lalala");
    CHECK_EQ(spl[2], "jjj");

    str.append(" kkk");
    spl = str.split(' ');
    CHECK_EQ(spl.size(), 4);
    CHECK_EQ(spl[0], "hello");
    CHECK_EQ(spl[1], "lalala");
    CHECK_EQ(spl[2], "jjj");
    CHECK_EQ(spl[3], "kkk");
}

TEST_CASE("vector_of_strings")
{
    igris::vector<igris::string> vec;
    igris::vector<igris::string> vec2;

    vec.push_back("lalala");
    vec.emplace_back("lalala");
    vec.emplace_back("lalala");
    vec.emplace_back("lalala");
    vec.emplace_back("lalala");
    vec.emplace_back("lalala");
    vec.emplace_back("lalala");
    vec.emplace_back("lalala");
    vec.emplace_back("lalala");
    vec.emplace_back("lalala");

    vec[1].append("kakakaka");
    vec[1].append("kakakaka");
    vec[1].append("kakakaka");
    vec[1].append("kakakaka");
    auto a = vec[1].split('a');
    a = vec[1].split('a');

    vec2 = vec;
    vec2 = vec;
}

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
    igris::vector<double> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    vec.push_back(4);
    vec.push_back(5);
    vec.push_back(6);
    vec.push_back(7);
    vec.push_back(8);
    vec.push_back(9);
    //    vec.erase(std::remove_if(
    //                  vec.begin(), vec.end(), [](int i) { return i % 2 == 0;
    //                  }),
    //              vec.end());

    //    CHECK_EQ(vec[0], 1);
    //    CHECK_EQ(vec[1], 3);
    //    CHECK_EQ(vec[2], 5);
    //    CHECK_EQ(vec[3], 7);
    //    CHECK_EQ(vec[4], 9);
}

TEST_CASE("static_string")
{
    igris::static_string<10> str;
    str.push_back('a');
    str.push_back('b');
    str.push_back('c');
    str.push_back('d');
    str.push_back('e');
    str.push_back('f');
    str.push_back('g');
    str.push_back('h');
    str.push_back('i');
    str.push_back('j');

    CHECK(str.size() == 10);
    CHECK(strcmp(str.c_str(), "abcdefghij") == 0);
}

TEST_CASE("static_vector")
{
    igris::static_vector<double, 8> vec1;
    vec1.push_back(1);
    vec1.push_back(2);
    vec1.push_back(3);
    igris::static_vector<double, 8> vec2;
    vec2.push_back(1);
    vec2.push_back(2);
    vec2.push_back(3);
    vec2.push_back(4);
    vec2.push_back(5);

    CHECK_EQ(vec1.size(), 3);
    CHECK_EQ(vec2.size(), 5);

    vec2 = vec1;
    CHECK_EQ(vec2.size(), 3);
    CHECK_EQ(vec2[0], 1);
    CHECK_EQ(vec2[1], 2);
    CHECK_EQ(vec2[2], 3);

    vec2.resize(2);
    CHECK_EQ(vec2.size(), 2);
}

TEST_CASE("vector -> static_vector")
{
    igris::vector<double> vec;
    vec.push_back(4);
    vec.push_back(5);
    vec.push_back(6);
    igris::static_vector<double, 8> svec;
    svec.push_back(1);
    svec.push_back(2);
    svec.push_back(3);

    svec.clear();
    std::copy(vec.begin(), vec.end(), std::back_inserter(svec));

    CHECK_EQ(svec[0], 4);
    CHECK_EQ(svec[2], 6);
}

TEST_CASE("static_string stoi")
{
    igris::static_string<16> str = "17";

    CHECK_EQ(igris::stoi(str), 17);
}
