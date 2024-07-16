#ifndef FULLY_CLOSED_SENSOR_H
#define FULLY_CLOSED_SENSOR_H

#include <nos/trent/trent.h>

class FullyClosedGear
{
public:
    // Count of pulses per units used by sensor.
    // Its a fabric constant.
    // 2**23 =  8_388_608 pulses per round for Renishou rotation encoder.
    // 2*10**7  = 20_000_000 pulses per meter for Renishou linear   encoder.
    double pulse_per_sensorunit = 0;

    // Conversion coefficient between sensor fabric unit and user unit.
    // user unit:
    // santimeter for linear. meters per santi  : 10**-2
    // degree for rotation    rounds per degree : 1/360
    double sensorunit_per_userunit = 0;

public:
    FullyClosedGear() = default;

    FullyClosedGear(double pulse_per_sensorunit, double sensorunit_per_userunit)
    {
        this->pulse_per_sensorunit = pulse_per_sensorunit;
        this->sensorunit_per_userunit = sensorunit_per_userunit;
    }

    double load_pulse_per_userunit() const
    {
        return pulse_per_sensorunit * sensorunit_per_userunit;
    }

    // Десериализует себя из объекта trent.
    void parse_trent(const nos::trent &tr)
    {
        pulse_per_sensorunit = tr["pulse_per_sensorunit"].as_numer_except();
        sensorunit_per_userunit =
            tr["sensorunit_per_userunit"].as_numer_except();
    }

    void set_configuration_by_name(const std::string &str)
    {
        if (str == "renishaw_8388608")
        {
            nos::println("Config renishaw_8388608");
            pulse_per_sensorunit = 8388608;
            sensorunit_per_userunit = 1. / 360.;
        }

        else if (str == "renishaw_20000000")
        {
            nos::println("Config renishaw_20000000");
            pulse_per_sensorunit = 20000000;
            sensorunit_per_userunit = 0.01;
        }

        else
            throw std::runtime_error("undefined fully_closed_sensor");
    }
};

#endif