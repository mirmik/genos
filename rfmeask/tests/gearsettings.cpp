#include "doctest/doctest.h"
#include <GearSettings.h>
#include <MitsubishiCommunicator.h>
#include <nos/trent/json.h>
#include <servos/MitsubishiServo.h>

MitsubishiCommunicator mrs;

TEST_CASE("gearsettings")
{
    SUBCASE("parse0")
    {
        auto tr = nos::json::parse(R"""(
			{
				"mul": 7,
				"gear1inputRatio": 6,
				"gear2num": 1,
				"gear2den": 1,
				"outputDiam": 6,
				"outputMul": 6,
				"beta": 19.5283,
				"twoLink": false
			}
		)""");

        GearSettings settings;
        settings.parse_trent(tr);
    }

    SUBCASE("eval_rounded_onelink")
    {
        auto tr = nos::json::parse(R"""(
			{
				"mul": 1,
				"gear1inputRatio": 50,
				"twoLink": false
			}
		)""");

        GearSettings settings;
        settings.parse_trent(tr);
        settings.set_is_linear(false);

        CHECK_EQ(settings.rounds_per_unit_rounded_onelink(), 50. * 1. / 360.);
        CHECK_EQ(settings.rounds_per_unit(), 50. * 1. / 360.);

        MitsubishiServo servo("test", nullptr, 2);
        servo.set_elgear(1000);

        CHECK_EQ(servo.command_unit_per_round(), 4194304. / 1000.);
        CHECK_EQ(
            int(servo.command_unit_per_round() * settings.rounds_per_unit() +
                0.5),
            583);
    }

    SUBCASE("eval_rounded_twolink")
    {
        auto tr = nos::json::parse(R"""(
			{
				"mul": 1,
				"gear1inputRatio": 50,
				"gear2num": 8,
				"gear2den": 13,
				"beta": 19.5283,
				"twoLink": true
			}
		)""");

        GearSettings settings;
        settings.parse_trent(tr);
        settings.set_is_linear(false);

        double res = 50. * 1. * 8. / 13. / 360.;
        CHECK_EQ(settings.rounds_per_unit_rounded_twolink(), res);
        CHECK_EQ(settings.rounds_per_unit(), res);

        MitsubishiServo servo("test", nullptr, 2);
        servo.set_elgear(1000);

        CHECK_EQ(servo.command_unit_per_round(), 4194304. / 1000.);
        CHECK_EQ(
            int(servo.command_unit_per_round() * settings.rounds_per_unit() +
                0.5),
            358);
    }

    SUBCASE("eval_linear_onelink")
    {
        auto tr = nos::json::parse(R"""(
			{
				"mul": 1,
				"gear1inputRatio": 50,
				"outputDiam": 17,
				"outputMul": 2,
				"beta": 19.5283,
				"twoLink": false
			}
		)""");

        GearSettings settings;
        settings.parse_trent(tr);
        settings.set_is_linear(true);

        double res =
            1. * 50. * std::cos((M_PI / 180) * 19.5283) * 10 / (M_PI * 17 * 2);
        CHECK_EQ(settings.rounds_per_unit_linear_onelink(), res);
        CHECK_EQ(settings.rounds_per_unit(), res);

        MitsubishiServo servo("test", nullptr, 2);
        servo.set_elgear(1000);

        CHECK_EQ(servo.command_unit_per_round(), 4194304. / 1000.);
        CHECK_EQ(
            int(servo.command_unit_per_round() * settings.rounds_per_unit() +
                0.5),
            18504);
    }

    SUBCASE("eval_linear_twolink")
    {
        auto tr = nos::json::parse(R"""(
			{
				"mul": 1,
				"gear1inputRatio": 50,
				"gear2num": 8,
				"gear2den": 13,
				"outputDiam": 17,
				"outputMul": 2,
				"beta": 19.5283,
				"twoLink": true
			}
		)""");

        GearSettings settings;
        settings.parse_trent(tr);
        settings.set_is_linear(true);

        double res = 1. * 50. * 8. / 13. * std::cos((M_PI / 180) * 19.5283) *
                     10 / (M_PI * 17 * 2);
        CHECK_EQ(settings.rounds_per_unit_linear_twolink(), res);
        CHECK_EQ(settings.rounds_per_unit(), res);

        MitsubishiServo servo("test", nullptr, 2);
        servo.set_elgear(1000);

        CHECK_EQ(servo.command_unit_per_round(), 4194304. / 1000.);
        CHECK_EQ(
            int(servo.command_unit_per_round() * settings.rounds_per_unit() +
                0.5),
            11387);
    }
}
