#include "doctest/doctest.h"
#include <GearSettings.h>
#include <nos/trent/json.h>

TEST_CASE("gearsettings_v2_style")
{
    auto tr = nos::json::parse(R"""(
			{
                "type": "array",
                "links": [
                    {
                        "type": "reductor",
                        "mul": 100
                    }
                ]
			}
		)""");

    GearSettings settings;
    settings.parse_trent(tr);

    CHECK_EQ(settings.rounds_per_unit(), 100);
    CHECK_EQ(settings.units_to_rounds(1), 100);
}

TEST_CASE("gearsettings_v2_style")
{
    auto tr = nos::json::parse(R"""(
			{
                "type": "array",
                "links": [
                    {
                        "type": "reductor",
                        "mul": 360
                    },
                    {
                        "type" : "converter_rev_degree"
                    }
                ]
			}
		)""");

    GearSettings settings;
    settings.parse_trent(tr);

    CHECK_EQ(settings.units_to_rounds(1), 1);
}

TEST_CASE("gearsettings_v2_style two links")
{
    auto tr1 = nos::json::parse(R"""(
			{
                "type": "array",
                "links": [
                    {
                        "type": "reductor",
                        "mul": 100
                    },
                    {
                        "type": "rational",
                        "num": 13,
                        "den": 5
                    },
                    {
                        "type": "output",
                        "num": 1,
                        "den": 360,
                        "type": "rotate"
                    }
                ]
			}
		)""");

    GearSettings settings1;
    settings1.parse_trent(tr1);
    double settings1_gear = settings1.rounds_per_unit();

    auto tr2 = nos::json::parse(R"""(
			{
            	"mul": 1,
				"gear1inputRatio": 100,
				"gear2num": 13,
				"gear2den": 5,
				"outputDiam": 1,
				"outputMul": 1,
				"beta": 19.5283,
				"twoLink": true
			}
		)""");

    GearSettings settings2;
    settings2.set_is_linear(false);
    settings2.parse_trent(tr2);
    double settings2_gear = settings2.rounds_per_unit();

    CHECK_EQ(settings1_gear, settings2_gear);
    CHECK_LT(
        std::abs(settings1.units_to_rounds(1) - settings2.units_to_rounds(1)),
        1e-5);
    CHECK_EQ(settings1.rounds_to_units(1), settings2.rounds_to_units(1));
}

TEST_CASE("gearsettings_v2_style two links. linear")
{
    auto tr1 = nos::json::parse(R"""(
			{
                "type": "array",
                "links": [
                    {
                        "type": "reductor",
                        "mul": 100
                    },
                    {
                        "type": "rational",
                        "num": 13,
                        "den": 5
                    },
                    {
                        "type": "conical",
                        "tooth": 18,
                        "mod": 2,
                        "beta": 19.5283
                    },
                    {
                        "type": "output",
                        "num": 10,
                        "den": 1,
                        "type": "linear"
                    }
                ]
			}
		)""");

    GearSettings settings1;
    settings1.parse_trent(tr1);
    double settings1_gear = settings1.rounds_per_unit();

    auto tr2 = nos::json::parse(R"""(
			{
            	"mul": 1,
				"gear1inputRatio": 100,
				"gear2num": 13,
				"gear2den": 5,
				"outputDiam": 18,
				"outputMul": 2,
				"beta": 19.5283,
				"twoLink": true
			}
		)""");

    GearSettings settings2;
    settings2.set_is_linear(true);
    settings2.parse_trent(tr2);
    double settings2_gear = settings2.rounds_per_unit();

    CHECK_EQ(settings1_gear, settings2_gear);
    CHECK_LT(settings1.units_to_rounds(1) - settings2.units_to_rounds(1), 1e-5);
    CHECK_LT(settings1.rounds_to_units(1) - settings2.rounds_to_units(1), 1e-5);
}

TEST_CASE("gearsettings_v2_style two links. linear")
{
    auto tr1 = nos::json::parse(R"""(
			{
                "type": "array",
                "links": [
                    {
                        "screw_pitch": 1,
                        "type": "screw"
                    },
                    {
                        "type": "lever",
                        "DX" : 425.7,
                        "DY" : 428,
                        "A" : 339,
                        "B" : 75,
                        "l_nominal" : 336
                    }
                ]
			}
		)""");

    GearSettings settings1;
    settings1.parse_trent(tr1);
    double pos1 = settings1.units_to_rounds(45 / 180.0 * M_PI);
    double pos2 = settings1.units_to_rounds(-5 / 180.0 * M_PI);
    double pos3 = settings1.units_to_rounds(0 / 180.0 * M_PI);
    double pos4 = settings1.rounds_to_units(363.4912555816);
    double pos5 = settings1.rounds_to_units(628.891563343);
    double pos6 = settings1.rounds_to_units(337.2575047739);

    CHECK_LT(std::abs(pos1 - 628.891563343), 1e-5);
    CHECK_LT(std::abs(pos2 - 337.2575047739), 1e-5);
    CHECK_LT(std::abs(pos3 - 363.4912555816), 1e-5);
    CHECK_LT(std::abs(pos4 - 0), 1e-5);
    CHECK_LT(std::abs(pos5 - 45 / 180.0 * M_PI), 1e-5);
    CHECK_LT(std::abs(pos6 - -5 / 180.0 * M_PI), 1e-5);
}

TEST_CASE("lever_arm")
{
    LeverArmGearLink link(425.7, 428, 339, 75, 336);

    double pos1 = link.output_to_input_conversion(45 / 180.0 * M_PI);
    double pos2 = link.output_to_input_conversion(-5 / 180.0 * M_PI);
    double pos3 = link.output_to_input_conversion(0 / 180.0 * M_PI);
    double pos4 = link.input_to_output_conversion(363.4912555816);
    double pos5 = link.input_to_output_conversion(628.891563343);
    double pos6 = link.input_to_output_conversion(337.2575047739);

    double pos7 = link.input_to_output_conversion(
        link.output_to_input_conversion(45 / 180.0 * M_PI));

    double pos8 = link.output_to_input_conversion(
        link.input_to_output_conversion(363.4912555816));

    CHECK_LT(std::abs(pos1 - 628.891563343), 1e-5);
    CHECK_LT(std::abs(pos2 - 337.2575047739), 1e-5);
    CHECK_LT(std::abs(pos3 - 363.4912555816), 1e-5);
    CHECK_LT(std::abs(pos4 - 0), 1e-5);
    CHECK_LT(std::abs(pos5 - 45 / 180.0 * M_PI), 1e-5);
    CHECK_LT(std::abs(pos6 - -5 / 180.0 * M_PI), 1e-5);

    CHECK_LT(std::abs(pos7 - 45 / 180.0 * M_PI), 1e-5);
    CHECK_LT(std::abs(pos8 - 363.4912555816), 1e-5);
}

TEST_CASE("lever")
{
    auto tr1 = nos::json::parse(R"""(
			{
                "links": [
                    {
                        "type": "converter_rad_degree"
                    }
                ],
                "type": "array"
            }
		)""");

    GearSettings settings1;
    settings1.parse_trent(tr1);

    CHECK_LT(std::abs(settings1.units_to_rounds(360) - 2 * M_PI), 1e-5);
}

TEST_CASE("lever")
{
    auto tr1 = nos::json::parse(R"""(
			{
                "fully_closed_sensor": "renishaw_8388608",
                "links": [
                    {
                        "screw_pitch": 4,
                        "type": "screw"
                    },
                    {
                        "offset": 363.491,
                        "type": "offset"
                    },
                    {
                        "A": 339,
                        "B": 75,
                        "DX": 425.7,
                        "DY": 428,
                        "l_nominal": 363.491,
                        "type": "lever"
                    },
                    {
                        "type": "converter_rad_degree"
                    }
                ],
                "type": "array"
            }
		)""");

    GearSettings settings1;
    settings1.parse_trent(tr1);

    CHECK_LT(std::abs(settings1.units_to_rounds(0) - 0), 1e-4);
    CHECK_LT(std::abs(settings1.units_to_rounds(45) - 66.3501398431), 1e-4);
    CHECK_LT(std::abs(settings1.rounds_per_unit() - 1.36442 + 0.0044779744),
             1e-5);

    CHECK_LT(std::abs(settings1.units_to_rounds(0) - 0), 1e-4);
    CHECK_LT(std::abs(settings1.units_to_rounds(10) - 14.2426), 1e-4);
    CHECK_LT(std::abs(settings1.units_to_rounds(20) - 29.2347), 1e-4);
    CHECK_LT(std::abs(settings1.units_to_rounds(30) - 44.3492), 1e-4);
    CHECK_LT(std::abs(settings1.units_to_rounds(40) - 59.1590), 1e-4);
}
