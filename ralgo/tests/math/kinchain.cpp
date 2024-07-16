#include <doctest/doctest.h>
#include <ralgo/kinematic/kinchain.h>

#include <igris/dprint.h>
#include <iostream>

namespace linalg
{
    using linalg::ostream_overloads::operator<<;
}

TEST_CASE("kinchain.1")
{
    ralgo::pose3<double> constants[3] = {{}, {}, {}};
    ralgo::screw3<double> locsenses[2] = {{{0, 0, 0}, {0, 0, 1}},
                                          {{0, 0, 0}, {0, 0, 0}}};
    double coords[2] = {0, 0};
    ralgo::screw3<double> outsenses[2];

    ralgo::kinematic_chain_sensivities(
        constants, locsenses, coords, 2, outsenses);

    CHECK_EQ(outsenses[0].lin, linalg::vec<double, 3>{0, 0, 1});
    CHECK_EQ(outsenses[0].ang, linalg::vec<double, 3>{0, 0, 0});
    CHECK_EQ(outsenses[1].lin, linalg::vec<double, 3>{0, 0, 0});
    CHECK_EQ(outsenses[1].ang, linalg::vec<double, 3>{0, 0, 0});
}

TEST_CASE("kinchain.2")
{
    ralgo::pose3<double> constants[3] = {
        ralgo::mov3<double>({1, 1, 1}), {}, {}};
    ralgo::screw3<double> locsenses[2] = {{{0, 0, 0}, {0, 0, 1}},
                                          {{0, 0, 0}, {0, 0, 0}}};
    double coords[2] = {0, 0};
    ralgo::screw3<double> outsenses[2];

    ralgo::kinematic_chain_sensivities(
        constants, locsenses, coords, 2, outsenses);

    CHECK_EQ(outsenses[0].lin, linalg::vec<double, 3>{0, 0, 1});
    CHECK_EQ(outsenses[0].ang, linalg::vec<double, 3>{0, 0, 0});
    CHECK_EQ(outsenses[1].lin, linalg::vec<double, 3>{0, 0, 0});
    CHECK_EQ(outsenses[1].ang, linalg::vec<double, 3>{0, 0, 0});
}

TEST_CASE("kinchain.3")
{
    ralgo::pose3<double> constants[2] = {ralgo::mov3<double>({0, 0, 0}),
                                         ralgo::mov3<double>({1, 0, 0})};
    ralgo::screw3<double> locsenses[1] = {{{0, 0, 1}, {0, 0, 0}}};
    double coords[1] = {0};
    ralgo::screw3<double> outsenses[1];

    ralgo::kinematic_chain_sensivities(
        constants, locsenses, coords, 1, outsenses);

    CHECK_EQ(outsenses[0].lin, linalg::vec<double, 3>{0, 1, 0});
    CHECK_EQ(outsenses[0].ang, linalg::vec<double, 3>{0, 0, 1});
}

TEST_CASE("kinchain.4")
{
    ralgo::pose3<double> constants[2] = {ralgo::mov3<double>({0, 0, 0}),
                                         ralgo::mov3<double>({1, 0, 0})};
    ralgo::screw3<double> locsenses[1] = {{{0, 0, 1}, {0, 0, 0}}};
    double coords[1] = {M_PI / 2};
    ralgo::screw3<double> outsenses[1];

    ralgo::kinematic_chain_sensivities(
        constants, locsenses, coords, 1, outsenses);

    CHECK_EQ(outsenses[0].lin[0], doctest::Approx(-1));
    CHECK_EQ(outsenses[0].lin[1], doctest::Approx(0));
    CHECK_EQ(outsenses[0].lin[2], doctest::Approx(0));
    CHECK_EQ(outsenses[0].ang, linalg::vec<double, 3>{0, 0, 1});
}

TEST_CASE("kinchain.5")
{
    ralgo::pose3<double> constants[2] = {
        ralgo::rot3<double>({0, 0, 1}, M_PI / 2),
        ralgo::mov3<double>({1, 0, 0})};
    ralgo::screw3<double> locsenses[1] = {{{0, 0, 1}, {0, 0, 0}}};
    double coords[1] = {M_PI / 2};
    ralgo::screw3<double> outsenses[1];

    ralgo::kinematic_chain_sensivities(
        constants, locsenses, coords, 1, outsenses);

    CHECK_EQ(outsenses[0].lin[0], doctest::Approx(0));
    CHECK_EQ(outsenses[0].lin[1], doctest::Approx(-1));
    CHECK_EQ(outsenses[0].lin[2], doctest::Approx(0));
    CHECK_EQ(outsenses[0].ang, linalg::vec<double, 3>{0, 0, 1});
}

TEST_CASE("kinchain.6")
{
    ralgo::pose3<double> constants[3] = {ralgo::mov3<double>({1, 0, 0}),
                                         ralgo::mov3<double>({1, 0, 0}),
                                         ralgo::mov3<double>({1, 0, 0})};

    ralgo::screw3<double> locsenses[2] = {{{0, 0, 1}, {0, 0, 0}},
                                          {{0, 0, 1}, {0, 0, 0}}};

    double coords[2] = {M_PI / 4, M_PI / 4};
    ralgo::screw3<double> outsenses[2];

    ralgo::kinematic_chain_sensivities(
        constants, locsenses, coords, 2, outsenses);

    CHECK_EQ(outsenses[0].lin[0], doctest::Approx(-(1 + 1 / sqrt(2))));
    CHECK_EQ(outsenses[0].lin[1], doctest::Approx(1 / sqrt(2)));
    CHECK_EQ(outsenses[0].lin[2], doctest::Approx(0));
    CHECK_EQ(outsenses[0].ang[0], doctest::Approx(0));
    CHECK_EQ(outsenses[0].ang[1], doctest::Approx(0));
    CHECK_EQ(outsenses[0].ang[2], doctest::Approx(1));

    CHECK_EQ(outsenses[1].lin[0], doctest::Approx(-1));
    CHECK_EQ(outsenses[1].lin[1], doctest::Approx(0));
    CHECK_EQ(outsenses[1].lin[2], doctest::Approx(0));
    CHECK_EQ(outsenses[1].ang[0], doctest::Approx(0));
    CHECK_EQ(outsenses[1].ang[1], doctest::Approx(0));
    CHECK_EQ(outsenses[1].ang[2], doctest::Approx(1));
}
