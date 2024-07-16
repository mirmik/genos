#include <doctest/doctest.h>
#include <ralgo/cnc/interpreter.h>

TEST_CASE("1")
{
    {
        ralgo::vector<cnc_float_type> direction = {1, 0, 0};
        cnc_float_type absolute_maximum = 0;
        igris::static_vector<cnc_float_type, NMAX_AXES> element_maximums = {
            10, 10, 10};
        auto result = cnc::interpreter::evaluate_external_accfeed(
            direction, absolute_maximum, element_maximums);
        CHECK_EQ(result, 10);
    }
    {
        ralgo::vector<cnc_float_type> direction = {1, 0, 0};
        cnc_float_type absolute_maximum = 10;
        igris::static_vector<cnc_float_type, NMAX_AXES> element_maximums = {
            10, 10, 10};
        auto result = cnc::interpreter::evaluate_external_accfeed(
            direction, absolute_maximum, element_maximums);
        CHECK_EQ(result, 10);
    }
    {
        ralgo::vector<cnc_float_type> direction = {1, 0, 0};
        cnc_float_type absolute_maximum = 9;
        igris::static_vector<cnc_float_type, NMAX_AXES> element_maximums = {
            10, 10, 10};
        auto result = cnc::interpreter::evaluate_external_accfeed(
            direction, absolute_maximum, element_maximums);
        CHECK_EQ(result, 9);
    }
    {
        ralgo::vector<cnc_float_type> direction = {1, 0, 0};
        cnc_float_type absolute_maximum = 11;
        igris::static_vector<cnc_float_type, NMAX_AXES> element_maximums = {
            10, 10, 10};
        auto result = cnc::interpreter::evaluate_external_accfeed(
            direction, absolute_maximum, element_maximums);
        CHECK_EQ(result, 10);
    }
}

TEST_CASE("2")
{
    {
        ralgo::vector<cnc_float_type> direction = {1, 0, 0};
        cnc_float_type absolute_maximum = 0;
        igris::static_vector<cnc_float_type, NMAX_AXES> element_maximums = {
            10, 10, 10};
        auto result = cnc::interpreter::evaluate_external_accfeed_2(
            direction, absolute_maximum, element_maximums);
        CHECK_EQ(result, 10);
    }

    {
        ralgo::vector<cnc_float_type> direction = {1, 0, 0};
        cnc_float_type absolute_maximum = 10;
        igris::static_vector<cnc_float_type, NMAX_AXES> element_maximums = {
            10, 10, 10};
        auto result = cnc::interpreter::evaluate_external_accfeed_2(
            direction, absolute_maximum, element_maximums);
        CHECK_EQ(result, 10);
    }
    {
        ralgo::vector<cnc_float_type> direction = {1, 0, 0};
        cnc_float_type absolute_maximum = 9;
        igris::static_vector<cnc_float_type, NMAX_AXES> element_maximums = {
            10, 10, 10};
        auto result = cnc::interpreter::evaluate_external_accfeed_2(
            direction, absolute_maximum, element_maximums);
        CHECK_EQ(result, 9);
    }
    {
        ralgo::vector<cnc_float_type> direction = {1, 0, 0};
        cnc_float_type absolute_maximum = 11;
        igris::static_vector<cnc_float_type, NMAX_AXES> element_maximums = {
            10, 10, 10};
        auto result = cnc::interpreter::evaluate_external_accfeed_2(
            direction, absolute_maximum, element_maximums);
        CHECK_EQ(result, 10);
    }
}

TEST_CASE("eq")
{
    {
        igris::static_vector<cnc_float_type, NMAX_AXES> element_maximums = {
            10, 10, 10};
        ralgo::vector<cnc_float_type> direction =
            ralgo::vecops::normalize(ralgo::vector<cnc_float_type>{1, 2, 3});
        cnc_float_type absolute_maximum = 0;
        auto result1 = cnc::interpreter::evaluate_external_accfeed(
            direction, absolute_maximum, element_maximums);
        auto result2 = cnc::interpreter::evaluate_external_accfeed_2(
            direction, absolute_maximum, element_maximums);
        CHECK(std::abs(result1 - result2) < 1e-5);
    }

    {
        igris::static_vector<cnc_float_type, NMAX_AXES> element_maximums = {
            10, 0, 10};
        ralgo::vector<cnc_float_type> direction =
            ralgo::vecops::normalize(ralgo::vector<cnc_float_type>{1, 0, 3});
        cnc_float_type absolute_maximum = 0;
        auto result1 = cnc::interpreter::evaluate_external_accfeed(
            direction, absolute_maximum, element_maximums);
        auto result2 = cnc::interpreter::evaluate_external_accfeed_2(
            direction, absolute_maximum, element_maximums);
        CHECK(std::abs(result1 - result2) < 1e-5);
    }

    {
        igris::static_vector<cnc_float_type, NMAX_AXES> element_maximums = {
            10, 0, 10};
        ralgo::vector<cnc_float_type> direction =
            ralgo::vecops::normalize(ralgo::vector<cnc_float_type>{1, 0, 3});
        cnc_float_type absolute_maximum = 10;
        auto result1 = cnc::interpreter::evaluate_external_accfeed(
            direction, absolute_maximum, element_maximums);
        auto result2 = cnc::interpreter::evaluate_external_accfeed_2(
            direction, absolute_maximum, element_maximums);
        CHECK(std::abs(result1 - result2) < 1e-5);
    }
}